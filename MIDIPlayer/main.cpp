#include "stdafx.h"
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <thread>

class MIDIPlayer
{
public:
	struct Volume {
		explicit Volume(uint8_t data = 0) : data(data) { assert(data <= 100); }
		uint8_t data;
	};
	struct Channel {
		explicit Channel(uint8_t data = 0) : data(data) { assert(data <= 0xf); }
		uint8_t data;
	};
	struct Patch {
		explicit Patch(uint8_t data = 0) : data(data) { assert(data <= 0x7f); }
		uint8_t data;
	};
	struct Pitch {
		explicit Pitch(uint8_t data = 0) : data(data) { assert(true); }
		uint8_t data;
	};
	struct DeTime {
		explicit DeTime(uint32_t data = 0) : data(data) { assert(true); }
		uint32_t data;
	};

public:
	using MIDIDevice = HMIDIOUT;
	class MIDIPlayerChannel
	{
	public:
		MIDIPlayerChannel(MIDIPlayer &player, Channel channel, Volume volume)
			: player(player), channel(channel), volume(volume) {}

		void SetVolume(Volume volume) {
			this->volume = volume;
		}
		void PlayPitch(Pitch pitch) {
			player.PlayPitch(pitch, volume, channel);
		}
		void PlayPitch(Pitch pitch, Volume volume) {
			player.PlayPitch(pitch, volume, channel);
		}
		void ClosePitch(Pitch pitch) {
			player.ClosePitch(pitch, channel);
		}
		void ChangePatch(Patch patch) {
			player.ChangePatch(patch, channel);
		}

		void Sleep(DeTime detime) {
			player.Sleep(detime);
		}

	private:
		MIDIPlayer &player;
		Channel channel;
		Volume volume;
	};

public:
	MIDIPlayer() {
		if (midiOutOpen(&midi_device, 0, 0, 0, CALLBACK_NULL)) {
			midi_device = nullptr;
			printf("There is an error opening the default MIDI out device!\n");
		}
	}
	~MIDIPlayer() {
		if (midi_device)
			midiOutClose(midi_device);
	}

	void PlayPitch(Pitch pitch, Volume volume, Channel channel) {
		PlayPitch(midi_device, pitch, volume, channel);
	}
	void ClosePitch(Pitch pitch, Channel channel) {
		ClosePitch(midi_device, pitch, channel);
	}
	void ChangePatch(Patch patch, Channel channel) {
		ChangePatch(midi_device, patch, channel);
	}

	void Sleep(DeTime detime) {
		::Sleep(detime.data);
	}

	MIDIPlayerChannel Create(Channel channel) {
		return MIDIPlayerChannel(*this, channel, Volume(100));
	}
	MIDIPlayerChannel Create(Channel channel, Volume volume) {
		return MIDIPlayerChannel(*this, channel, volume);
	}
	MIDIPlayerChannel Create(Channel channel, Volume volume, Patch patch) {
		MIDIPlayerChannel pc(*this, channel, volume);
		pc.ChangePatch(patch);
		return pc;
	}
	MIDIPlayerChannel Create(Channel channel, Patch patch) {
		return Create(channel, Volume(100), patch);
	}
	MIDIPlayerChannel Create(Channel channel, Patch patch, Volume volume) {
		return Create(channel, volume, patch);
	}

public:
	static void PlayPitch(MIDIDevice midi_device, Pitch pitch, Volume volume, Channel channel) {
		midiOutShortMsg(midi_device, ((volume.data * 0x10000) | (pitch.data * 0x100) | (channel.data + 0x90)));
	}
	static void ClosePitch(MIDIDevice midi_device, Pitch pitch, Channel channel) {
		midiOutShortMsg(midi_device, ((pitch.data * 0x100) | (channel.data + 0x80)));
	}
	static void ChangePatch(MIDIDevice midi_device, Patch patch, Channel channel) {
		midiOutShortMsg(midi_device, ((patch.data * 0x100) | (channel.data + 0xc0)));
	}

private:
	MIDIDevice midi_device;
};

int main(int argc, const char *argv[])
{
	MIDIPlayer player;

	using Channel = MIDIPlayer::Channel;
	using Pitch = MIDIPlayer::Pitch;
	using Volume = MIDIPlayer::Volume;
	using Patch = MIDIPlayer::Patch;
	using DeTime = MIDIPlayer::DeTime;

	auto playerc0 = player.Create(Channel(0), Volume(100), Patch(0));
	auto playerc1 = player.Create(Channel(1), Volume(100), Patch(40));

	std::thread t1([&]() {
		playerc0.PlayPitch(Pitch(0x3c));
		Sleep(2000);
		playerc0.PlayPitch(Pitch(0x3c));
		Sleep(1000);
		playerc0.PlayPitch(Pitch(0x3c));
		Sleep(1000);
		playerc0.ClosePitch(Pitch(0x3c));
		});
	std::thread t2([&]() {
		Sleep(500);
		playerc1.PlayPitch(Pitch(0x43));
		Sleep(1000);
		playerc1.ClosePitch(Pitch(0x43));
		});

	t1.join();
	t2.join();

	return 0;
}
