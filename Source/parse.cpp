#include "parse.h"

MIDIPlayer::Pitch parsePitch(const char *word, const char *&end) {
	int num, i, j;
	char note[3];
	num = i = j = 0;
	//judeg the first character
	if (word[i] != '#' && word[i] != 'b' && !(word[i] <= 'G' && word[i] >= 'A')) {
		printf("Illegal operation.The first character is not the correct character.\n");
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	note[j] = word[i++];
	//save the next character when first one is # or b
	if (note[j] == '#' || note[j] == 'b') {
		if (word[i] <= 'G' && word[i] >= 'A') {
			note[++j] = word[i++];
		}
		else {
			printf("Illegal operation.The character is not the charater between A and G.\n");
			end = word;
			return (MIDIPlayer::Pitch(0));
		}
	}
	//save the num 
	if (word[i] == '-') {
		if (word[++i] == '1')
			num = -1;
		else {
			printf("Illegal operation.The number is under o but is not -1.\n");
			end = word;
			return (MIDIPlayer::Pitch(0));
		}
	}
	else if (word[i] <= '9' && word[i] >= '0')
		num = word[i] - '0';
	else {
		printf("Illegal operation.There should be a number between 0 and 9.\n");
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	//judge the corret or not again
	if (note[0] == 'b' && note[1] == 'C' && num == -1) {
		printf("Illegal operation.There is no bC-1.\n");
		end = word;
		return (MIDIPlayer::Pitch(0));
	}
	else if (num == 9) {
		if ((note[0] == '#' && (note[1] == 'G' || note[1] == 'A')) || (note[0] == 'A' || note[0] == 'B')) {
			printf("Illegal operation.There is no #G A #A B when num == 9\n");
			end = word;
			return (MIDIPlayer::Pitch(0));
		}

	}
	end = &word[++i];
	return MIDIPlayer::Pitch(MIDIPlayer::Note(word), MIDIPlayer::Octave(num));
}

MIDIPlayer::Notation parseNumberedNotation(const char *word, const char *&end) {
	int base = 0;
	int octave = 0;
	int i = 0;
	bool rise;
	while (word[i] == '-' || word[i] == '+' || word[i] <= '7' && word[i] >= '1') {
		if (word[i] == '-') {//if the first character is - 
			if (word[0] == '-')//if this character is -
				octave--;
			else {//if it is not ' either number of range
				printf("Illegal operation.The are wrong characters when first character is '-'.\n");
				end = word;
				return MIDIPlayer::Notation();
			}
		}
		else if (word[i] == '+') {
			if (word[0] == '+')//if this character is +
				octave++;
			else {//if it is not ' either number of range
				printf("Illegal operation.The are wrong characters when first character is '+'.\n");
				end = word;
				return MIDIPlayer::Notation();
			}
		}
		else {
			base = word[i] - '0';
			if (word[i + 1] == '#')
				rise = true;
			else
				rise = false;
			break;
		}
		i++;
	}
	if (!base) {
		printf("Illegal operation.There are no base number in this string or there are wrong characters in it\n");
		end = word;
		return MIDIPlayer::Notation();
	}
	//printf("base:%d\t octave:%d\t rise:%d\n", base, octave, rise);
	end = &word[octave + 1 + rise];
	return MIDIPlayer::Notation(MIDIPlayer::NotationBase(base, rise), MIDIPlayer::NotationOctave(octave));
}
