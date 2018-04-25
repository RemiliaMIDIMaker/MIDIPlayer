#include "midiplayer.h"
#include <cstdio>
#include "../ThirdLibrary/portmidi/include/portmidi.h"
#include "../ThirdLibrary/portmidi/include/porttime.h"

namespace MIDIPlayer
{
	static PortMidiStream*& getMIDIDevice(void* &data) {
		return reinterpret_cast<PortMidiStream*&>(data);
	}
	static PortMidiStream*& getMIDIDevice(Player::MIDIDevice &midi_device) {
		return getMIDIDevice(midi_device.get());
	}

	void Player::MIDIDevice::open(void* &data) {
		if (Pm_OpenOutput(&getMIDIDevice(data), Pm_GetDefaultOutputDeviceID(), nullptr, 0, nullptr, nullptr, 0)) {
			printf("There is an error opening the default MIDI out device!\n");
		}
	}
	void Player::MIDIDevice::close(void* data) {
		Pm_Close(getMIDIDevice(data));
	}
	void*& Player::MIDIDevice::get() {
		return data;
	}

	static void _Pm_WriteShort(PortMidiStream *stream, PmMessage message) {
		Pm_WriteShort(stream, 0, message);
	}

	void Player::PlayPitch(MIDIDevice midi_device, Pitch pitch, Volume volume, Track track) {
		_Pm_WriteShort(getMIDIDevice(midi_device), ((volume.data * 0x10000) | (pitch.data * 0x100) | (track.data + 0x90)));
	}
	void Player::ClosePitch(MIDIDevice midi_device, Pitch pitch, Track track) {
		_Pm_WriteShort(getMIDIDevice(midi_device), ((pitch.data * 0x100) | (track.data + 0x80)));
	}
	void Player::ChangePatch(MIDIDevice midi_device, Patch patch, Track track) {
		_Pm_WriteShort(getMIDIDevice(midi_device), ((patch.data * 0x100) | (track.data + 0xc0)));
	}
	void Player::Sleep(DeTime detime) {
		Pt_Sleep(detime.data);
	}
}
