#pragma once
#include "midiplayer.h"
#include "notation.h"

// Example : #C4 C#4
MIDIPlayer::Pitch parsePitch(const char *word, const char *&end);

// Example : 1 2# +1 --1
MIDIPlayer::Notation parseNumberedNotation(const char *word, const char *&end);

// Example : -- // //.
MIDIPlayer::NoteValue parseNoteValue(const char *word, const char *&end);
