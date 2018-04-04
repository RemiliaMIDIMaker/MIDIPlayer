#include "parse.h"
MIDIPlayer::Pitch parsePitch(const char *word, const char *&end) {
	int num, sum, i, j;
	char note[3];
	num = sum = i = j = 0;
	while (!(word[i] <= 'G'&&word[i] >= 'A') && word[i] != '#')
		i++;
	if (&word[i] == end) {
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	note[j] = word[i++];
	if (note[j] == '#') {
		if (word[i] <= 'G' && word[i] >= 'A') {
			if (word[i] == 'B' || word[i] == 'E') {
				end = word;
				return (MIDIPlayer::Pitch(0));
			}
			else
				note[++j] = word[i++];
		}
		else {
			end = word;
			return (MIDIPlayer::Pitch(0));
		}
	}
	if (&word[i] == end) {
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	if (word[i] == '-') {
		if (&word[++i] == end) {
			end = word;
			return (MIDIPlayer::Pitch(0));
		}
		if (word[i] == '1')
			num = -1;
		else {
			end = word;
			return (MIDIPlayer::Pitch(0));
		}
	}
	else if (word[i] <= '9' && word[i] >= '0')
		num = word[i] - '0';
	else {
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	switch (note[j]) {
	case 'B':sum += 2;
	case 'A':sum += 2;
	case 'G':sum += 2;
	case 'F':sum += 1;
	case 'E':sum += 2;
	case 'D':sum += 2;
	case 'C':break;
	}
	if (j == 1)
		sum += 1;
	sum += (1 + num) * 12;
	end = &word[++i];
	return MIDIPlayer::Pitch(MIDIPlayer::Note(word), MIDIPlayer::Octave(sum));
}
