#pragma once
#include <cstdint>
#include <cassert>
#include <memory>
#include <cctype>

namespace MIDIPlayer
{
	struct Volume {
		explicit Volume(uint8_t data = 0) : data(data) { assert(data <= 0x7f); }
		uint8_t data;
	};
	struct Track {
		explicit Track(uint8_t data = 0) : data(data) { assert(data <= 0xf); }
		uint8_t data;
	};
	struct Patch {
		explicit Patch(uint8_t data = 0) : data(data) { assert(data <= 0x7f); }
		uint8_t data;
	};
	struct Note {
		explicit Note(uint8_t data = 0) : data(data) {
			if ('A' <= data && data <= 'G') {
				if (data < 'C')
					this->data = (data - 'A') + 6;
				else
					this->data = (data - 'C') + 1;
			}
			else if (!(1 <= data && data <= 7)) {
				assert(false);
			}
		}
		uint8_t data;
	};
	struct KeySignature {
		explicit KeySignature(uint8_t data = 0) : data(data) {
			if (data == '#')
				this->data = +1;
			else if (data == 'b')
				this->data = -1;
			else if (data == 0)
				this->data = 0;
			else
				assert(false);
		}
		uint8_t data;
	};
	struct Octave {
		explicit Octave(uint8_t data = 0) : data(data) { assert(-1 <= data && data <= 9); }
		uint8_t data;
	};
	struct Pitch {
		explicit Pitch(uint8_t data) : data(data) { assert(data <= 0x7f); }
		Pitch(Note note) : Pitch(note, KeySignature(0), Octave(4)) {}
		Pitch(Note note, KeySignature keysignature, Octave octave) {
			// Middle C : 0x3c
			// C C# D D# E F F# G G# A A# B : 0xc
			// 1 1# 2 2# 3 4 4# 5 5# 6 6# 7
			static const uint8_t table[] = { 0x0c, 0x0e, 0x10, 0x11, 0x13, 0x15, 0x17 };
			this->data = table[note.data - 1] + keysignature.data + octave.data * 0xc;
		}
		Pitch(Note note, KeySignature keysignature) : Pitch(note, keysignature, Octave(4)) {}
		Pitch(Note note, Octave octave) : Pitch(note, KeySignature(0), octave) {}
		uint8_t data;
	};
	struct DeTime {
		explicit DeTime(uint32_t data = 0) : data(data) { assert(true); }
		uint32_t data;
	};

	class Player
	{
	public:
		class MIDIDevice {
		public:
			MIDIDevice() : ref(nullptr, close) { open(data); }

			void*& get();

		private:
			static void open(void* &data);
			static void close(void *data);
			void *data;
			std::shared_ptr<void> ref;
		};
		class TrackPlayer
		{
		public:
			TrackPlayer(Player &player, Track track, Volume volume)
				: player(player), track(track), volume(volume) {}

			void SetVolume(Volume volume) {
				this->volume = volume;
			}
			void PlayPitch(Pitch pitch) {
				player.PlayPitch(pitch, volume, track);
			}
			void PlayPitch(Pitch pitch, Volume volume) {
				player.PlayPitch(pitch, volume, track);
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
				player.ClosePitch(pitch, track);
			}
			void ChangePatch(Patch patch) {
				player.ChangePatch(patch, track);
			}

			void Sleep(DeTime detime) {
				player.Sleep(detime);
			}

		private:
			Player & player;
			Track track;
			Volume volume;
		};

	public:
		Player() = default;
		Player(const Player &) = default;

		void PlayPitch(Pitch pitch, Volume volume, Track track) {
			PlayPitch(midi_device, pitch, volume, track);
		}
		void ClosePitch(Pitch pitch, Track track) {
			ClosePitch(midi_device, pitch, track);
		}
		void ChangePatch(Patch patch, Track track) {
			ChangePatch(midi_device, patch, track);
		}

		void Sleep(DeTime detime);

		TrackPlayer Create(Track track) {
			return TrackPlayer(*this, track, Volume(0x7f));
		}
		TrackPlayer Create(Track track, Volume volume) {
			return TrackPlayer(*this, track, volume);
		}
		TrackPlayer Create(Track track, Volume volume, Patch patch) {
			TrackPlayer pc(*this, track, volume);
			pc.ChangePatch(patch);
			return pc;
		}
		TrackPlayer Create(Track track, Patch patch) {
			return Create(track, Volume(0x7f), patch);
		}
		TrackPlayer Create(Track track, Patch patch, Volume volume) {
			return Create(track, volume, patch);
		}

	public:
		static void PlayPitch(MIDIDevice midi_device, Pitch pitch, Volume volume, Track track);
		static void ClosePitch(MIDIDevice midi_device, Pitch pitch, Track track);
		static void ChangePatch(MIDIDevice midi_device, Patch patch, Track track);

	private:
		MIDIDevice midi_device;
	};
}
