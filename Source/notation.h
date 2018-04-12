#pragma once
#include "midiplayer.h"

namespace MIDIPlayer
{
	struct NotationBase { // 0, 1 ~ 14; (1 ~ 7, true/false)
		explicit NotationBase() : data(0) {}
		explicit NotationBase(uint8_t data, bool rise) : data((data * 2 - 1) + (rise ? 1 : 0)) { assert(1 <= data && data <= 7); }
		uint8_t data;
	};
	struct NotationOctave { // -11 ~ 10
		explicit NotationOctave(int8_t data = 0) : data(data) {}
		int8_t data;
	};
	struct Notation {
		explicit Notation() {}
		explicit Notation(NotationBase base, NotationOctave octave) : base(base), octave(octave) {}
		NotationBase base;
		NotationOctave octave;
	};

	struct Beat {
		// 1---  : 4   beats : 32
		// 1-    : 2   beats : 16
		// 1     : 1   beat  : 8
		// 1/    : 1/2 beat  : 4
		// 1//   : 1/4 beat  : 2
		// 1///  : 1/8 beat  : 1
		// 1.    : 1 + 1/2   : 8 + 4 = 12
		// 1/.   : 1/2 + 1/4 : 4 + 2 = 6

		explicit Beat(uint8_t data) : data(data) { assert(data != 0); }
		uint8_t data;
	};

	// if 1 = C4 then 1# = #C4, 2 = D4, 2# = #D4 ...
	// Notice : 3# == 4, 7# == +1, whatever 1 = what.
	Pitch offset(const Pitch &pitch, const Notation &notation, bool &success);
	inline Pitch operator+(const Pitch &pitch, const Notation &notation) {
		bool success;
		auto r = offset(pitch, notation, success);
		assert(success);
		return r;
	}
	inline Pitch operator+(const Notation &notation, const Pitch &pitch) {
		return pitch + notation;
	}
}
