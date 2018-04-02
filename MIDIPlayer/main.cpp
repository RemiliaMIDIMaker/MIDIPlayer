#include "stdafx.h"
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <thread>
#include <vector>

namespace MIDIPlayer
{
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

	class Player
	{
	public:
		using MIDIDevice = HMIDIOUT;
		class ChannelPlayer
		{
		public:
			ChannelPlayer(Player &player, Channel channel, Volume volume)
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
			void PlayPitch(Pitch pitch, DeTime detime) {
				PlayPitch(pitch);
				Sleep(detime);
				ClosePitch(pitch);
			}
			void PlayPitch(Pitch pitch, Volume volume, DeTime detime) {
				PlayPitch(pitch, volume);
				Sleep(detime);
				ClosePitch(pitch);
			}

			template <typename Iter>
			void PlayPitchs(Iter begin, Iter end) {
				for (Iter iter = begin; iter != end; ++iter) {
					PlayPitch(*iter);
				}
			}
			template <typename Iter>
			void ClosePitchs(Iter begin, Iter end) {
				for (Iter iter = begin; iter != end; ++iter) {
					ClosePitch(*iter);
				}
			}
			template <typename Iter>
			void PlayPitchs(Iter begin, Iter end, DeTime detime) {
				PlayPitchs(begin, end);
				Sleep(detime);
				ClosePitchs(begin, end);
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
			Player & player;
			Channel channel;
			Volume volume;
		};

	public:
		Player() {
			if (midiOutOpen(&midi_device, 0, 0, 0, CALLBACK_NULL)) {
				midi_device = nullptr;
				printf("There is an error opening the default MIDI out device!\n");
			}
		}
		~Player() {
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

		ChannelPlayer Create(Channel channel) {
			return ChannelPlayer(*this, channel, Volume(100));
		}
		ChannelPlayer Create(Channel channel, Volume volume) {
			return ChannelPlayer(*this, channel, volume);
		}
		ChannelPlayer Create(Channel channel, Volume volume, Patch patch) {
			ChannelPlayer pc(*this, channel, volume);
			pc.ChangePatch(patch);
			return pc;
		}
		ChannelPlayer Create(Channel channel, Patch patch) {
			return Create(channel, Volume(100), patch);
		}
		ChannelPlayer Create(Channel channel, Patch patch, Volume volume) {
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
}

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

	Player player;

	std::thread t1([&]() {
		auto playerc = player.Create(Channel(0), Volume(100), Patch(0));
		playerc.PlayPitch(Pitch(0x3c), DeTime(500));
		playerc.PlayPitch(Pitch(0x40), DeTime(500));
		playerc.PlayPitch(Pitch(0x43), DeTime(500));

		Pitch vecs[] { Pitch(0x3c), Pitch(0x40), Pitch(0x43) };
		playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		});

	std::thread t2([&]() {
		auto playerc = player.Create(Channel(1), Volume(100), Patch(0));
		playerc.Sleep(DeTime(500));
		playerc.PlayPitch(Pitch(0x43), DeTime(500));
		playerc.PlayPitch(Pitch(0x43), DeTime(500));
		playerc.PlayPitch(Pitch(0x43), DeTime(1000));
		});

	t1.join();
	t2.join();

	return 0;
}
