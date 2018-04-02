#include "stdafx.h"
#include <cstdio>
#include <cstdint>
#include <cassert>

class MIDIPlayer
{
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

	void PlayPitch(uint8_t pitch, uint8_t volume, uint8_t channel = 0) {
		assert(volume <= 100);
		assert(channel <= 0xf);
		midiOutShortMsg(midi_device, ((volume * 0x10000) | (pitch * 0x100) | (channel + 0x90)));
	}
	void ClosePitch(uint8_t pitch, uint8_t channel = 0) {
		assert(channel <= 0xf);
		midiOutShortMsg(midi_device, ((pitch * 0x100) | (channel + 0x80)));
	}

	void Sleep(uint32_t det) {
		::Sleep(det);
	}

private:
	HMIDIOUT midi_device;
};

int main(int argc, const char *argv[])
{
	MIDIPlayer player;

	player.PlayPitch(0x3c, 100);
	player.Sleep(1000);
	player.ClosePitch(0x3c);

	player.PlayPitch(0x40, 100);
	player.Sleep(1000);
	player.ClosePitch(0x40);

	player.PlayPitch(0x43, 100);
	player.Sleep(1000);
	player.ClosePitch(0x43);

	player.PlayPitch(0x3c, 100);
	player.PlayPitch(0x40, 100);
	player.PlayPitch(0x43, 100);
	player.Sleep(2000);
	player.ClosePitch(0x3c);
	player.ClosePitch(0x40);
	player.ClosePitch(0x43);

	return 0;
}
