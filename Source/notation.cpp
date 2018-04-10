#include "notation.h"

namespace MIDIPlayer
{
	Pitch offset(const Pitch &pitch, const Notation &notation, bool &success) {
		assert(notation.base.data != 0);
		static const int8_t table[] = { 0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12 };
		int offset = table[notation.base.data - 1] + 0xc * notation.octave.data;
		int ndata = pitch.data + offset;
		success = (0 <= ndata && ndata <= 0x7f);
		return (success) ? Pitch(ndata) : Pitch(0);
	}
}
