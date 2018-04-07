#pragma once
#include "midiplayer.h"

// Example : #C4 C#4
MIDIPlayer::Pitch parsePitch(const char *word, const char *&end);

// Example : 1 2# +1 --1
MIDIPlayer::Notation parseNumberedNotation(const char *word, const char *&end);
