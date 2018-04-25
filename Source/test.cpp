#include "parse.h"

static void Test1() {
	using namespace MIDIPlayer;

	char s;
	uint8_t v;
	char word[5] = "";
	const char *end;
	for (int o = -1; o <= 9; o++) {
		for (int i = 1; i <= 7; i++) {
			if (i == 6 || i == 7)
				s = 'A' + i - 6;
			else
				s = 'C' + i - 1;

			if (!(s == 'C' && o == -1)) {
				sprintf(word, "b%c%d", s, o);
				v = parsePitch(word, end).data;
				printf("%s 0x%02x\n", word, v);
				assert(v == Pitch(Note(NoteBase(s), KeySignature('b')), Octave(o)).data);
				assert(end == word + ((o == -1) ? 4 : 3));
			}
			sprintf(word, "%c%d", s, o);
			v = parsePitch(word, end).data;
			printf(" %s 0x%02x\n", word, v);
			assert(v == Pitch(Note(NoteBase(s), KeySignature('\0')), Octave(o)).data);
			assert(end == word + ((o == -1) ? 3 : 2));
			if (s == 'G' && o == 9)
				break;
			sprintf(word, "#%c%d", s, o);
			v = parsePitch(word, end).data;
			printf("%s 0x%02x\n", word, v);
			assert(v == Pitch(Note(NoteBase(s), KeySignature('#')), Octave(o)).data);
			assert(end == word + ((o == -1) ? 4 : 3));
		}
	}
}

static void Test2_Base(MIDIPlayer::Pitch base) {
	using namespace MIDIPlayer;
	char word[0x10] = "";

	for (int o = -11; o <= 10; o++) {
		for (int i = 1; i <= 7; i++) {
			bool success = true;
			const char *end;

			int x = 0;
			int count = o > 0 ? o : -o;
			while (count--)
				word[x++] = (o > 0 ? '+' : '-');
			word[x++] = i + '0';
			word[x] = '\0';

			{
				Notation notation(NotationBase(i, false), NotationOctave(o));
				Notation notation_p = parseNumberedNotation(word, end);
				assert(notation.base.data == notation_p.base.data);
				assert(notation.octave.data == notation_p.octave.data);
				assert(end == word + o + 1);
				printf("%s\n", word);
			}

			word[x++] = '#';
			word[x] = '\0';

			{
				Notation notation(NotationBase(i, true), NotationOctave(o));
				Notation notation_p = parseNumberedNotation(word, end);
				assert(notation.base.data == notation_p.base.data);
				assert(notation.octave.data == notation_p.octave.data);
				assert(end == word + o + 2);
				printf("%s\n", word);
			}
		}
	}
}

static void Test2()
{
	using namespace MIDIPlayer;
	Pitch base(Note("C"), Octave(-1));

	Test2_Base(base);
}

static void Test3()
{
	char buffer[0xff] = "";
	const char *end;

	while (true) {
		printf(">");
		scanf("%s", buffer);
		auto v = parseNoteValue(buffer, end);
		printf("%d %lld\n", v.data, end - buffer);
	}
}

void Test()
{
	//Test1();
	//Test2();
	//Test3();
}
