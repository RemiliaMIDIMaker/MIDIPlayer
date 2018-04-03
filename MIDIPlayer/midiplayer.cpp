#include "midiplayer.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <cstdio>
#pragma comment(lib, "Winmm.lib")

namespace MIDIPlayer
{
	static HMIDIOUT& getMIDIDevice(void* &data) {
		return reinterpret_cast<HMIDIOUT&>(data);
	}
	static HMIDIOUT& getMIDIDevice(Player::MIDIDevice &midi_device) {
		return getMIDIDevice(midi_device.get());
	}

	void Player::MIDIDevice::open(void* &data) {
		if (midiOutOpen(&getMIDIDevice(data), 0, 0, 0, CALLBACK_NULL)) {
			printf("There is an error opening the default MIDI out device!\n");
		}
	}
	void Player::MIDIDevice::close(void* data) {
		midiOutClose(getMIDIDevice(data));
	}
	void*& Player::MIDIDevice::get() {
		return data;
	}

	void Player::PlayPitch(MIDIDevice midi_device, Pitch pitch, Volume volume, Channel channel) {
		midiOutShortMsg(getMIDIDevice(midi_device), ((volume.data * 0x10000) | (pitch.data * 0x100) | (channel.data + 0x90)));
	}
	void Player::ClosePitch(MIDIDevice midi_device, Pitch pitch, Channel channel) {
		midiOutShortMsg(getMIDIDevice(midi_device), ((pitch.data * 0x100) | (channel.data + 0x80)));
	}
	void Player::ChangePatch(MIDIDevice midi_device, Patch patch, Channel channel) {
		midiOutShortMsg(getMIDIDevice(midi_device), ((patch.data * 0x100) | (channel.data + 0xc0)));
	}
	void Player::Sleep(DeTime detime) {
		::Sleep(detime.data);
	}
}
