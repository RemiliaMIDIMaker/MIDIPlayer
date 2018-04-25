#pragma once
#include <cstdint>
#include <cassert>
#include <memory>
#include <cctype>

namespace MIDIPlayer
{
	// Pitchs
	// * Note
	//     NoteBase + KeySignature -> Note
	//     Note + Octave -> Picth
	// * Notation
	//     Notation + Pitch(Note + Octave) -> Picth

	struct NoteBase { // 7 notes
		// * 'C' 'D' 'E' 'F' 'G' 'A' 'B' 
		// *  1   2   3   4   5   6   7
		explicit NoteBase(uint8_t data = 0) {
			if ('A' <= data && data <= 'G') {
				if (data < 'C')
					this->data = (data - 'A') + 6;
				else
					this->data = (data - 'C') + 1;
			}
			else if (1 <= data && data <= 7) {
				this->data = data;
			}
			else {
				assert(false);
			}
		}
		uint8_t data;
	};
	struct KeySignature {
		// * '\0' '#' 'b'
		// *   0   1  -1
		explicit KeySignature(int8_t data = 0) : data(data) {
			if (data == '#')
				this->data = +1;
			else if (data == 'b')
				this->data = -1;
			else if (data == 0)
				this->data = 0;
			else
				assert(false);
		}
		int8_t data;
	};
	struct Note { // 12 + 2 notes (0 1 ~ 12 13) bC(-B) C #C(bD) D #D(bE) E(bF) F(#E) #F(bG) G #G(bA) A #A(bB) B #B(+C)
		explicit Note(uint8_t data) /* 0 ~ 11 : C ~ B */ : data(data + 1) { assert(data < 12); }
		explicit Note(NoteBase notebase) : Note(notebase, KeySignature()) {}
		explicit Note(NoteBase notebase, KeySignature keysignature) {
			static const int8_t table[] = { 1, 3, 5, 6, 8, 10, 12 };
			this->data = table[notebase.data - 1] + keysignature.data;
		}
		explicit Note(const char *word) { // X bX #X Xb X#
			if (word[0] == 'b' || word[0] == '#')
				this->data = Note(NoteBase(word[1]), KeySignature(word[0])).data;
			else if (word[1] == 'b' || word[1] == '#')
				this->data = Note(NoteBase(word[0]), KeySignature(word[1])).data;
			else
				this->data = Note(NoteBase(word[0])).data;
		}
		uint8_t data;
	};
	struct Octave {
		static constexpr int8_t default_octave = 4;
		explicit Octave(int8_t data = default_octave) : data(data) { assert(-1 <= data && data <= 9); }
		int8_t data;
	};

	struct Pitch {
		explicit Pitch(uint8_t data) : data(data) { assert(data <= 0x7f); }
		Pitch(Note note) : Pitch(note, Octave()) {}
		Pitch(Note note, Octave octave) {
			// Middle C : 0x3c
			// C #C D #D E F #F G #G A #A B : 0xc
			// 1 #1 2 #2 3 4 #4 5 #5 6 #6 7
			this->data = note.data + 0xc - 1 + octave.data * 0xc;
			assert(this->data <= 0x7f);
		}
		uint8_t data;
	};

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
		Player();
		Player(const Player &);

		~Player();

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
