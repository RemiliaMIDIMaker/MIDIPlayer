#include "midiplayer.h"
#include "parse.h"
#include <thread>
#include <vector>

void Test1() {
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

void Test2_Base(MIDIPlayer::Pitch base) {
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
				Notation notation(NotationBase(i, false), NotationOctave(o));
				Notation notation_p = parseNumberedNotation(word, end);
				assert(notation.base.data == notation_p.base.data);
				assert(notation.octave.data == notation_p.octave.data);
				assert(end == word + o + 2);
				printf("%s\n", word);
			}
		}
	}
}

void Test2()
{
	using namespace MIDIPlayer;
	Pitch base(Note("C"), Octave(-1));

	Test2_Base(base);
}

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

	Test1();
	Test2();

	Player player;

	bool play_end = false;
	std::thread t1([&]() {
		auto playerc = player.Create(Track(0));

		Pitch base(Note("C"));

		Notation not1(NotationBase(1, false), NotationOctave(0));
		Notation not2(NotationBase(2, false), NotationOctave(0));
		Notation not3(NotationBase(3, false), NotationOctave(0));
		Notation not4(NotationBase(4, false), NotationOctave(0));
		Notation not5(NotationBase(5, false), NotationOctave(0));

		playerc.PlayPitch(Pitch(not1, base), DeTime(500));
		playerc.PlayPitch(Pitch(not1, base), DeTime(500));
		playerc.PlayPitch(Pitch(not3, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(1000));

		playerc.PlayPitch(Pitch(not5, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(1000));
		playerc.PlayPitch(Pitch(not3, base), DeTime(500));
		playerc.PlayPitch(Pitch(not3, base), DeTime(1000));

		playerc.PlayPitch(Pitch(not1, base), DeTime(500));
		playerc.PlayPitch(Pitch(not1, base), DeTime(500));
		playerc.PlayPitch(Pitch(not3, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(1000));

		playerc.PlayPitch(Pitch(not5, base), DeTime(500));
		playerc.PlayPitch(Pitch(not5, base), DeTime(1000));
		playerc.PlayPitch(Pitch(not4, base), DeTime(500));
		playerc.PlayPitch(Pitch(not4, base), DeTime(1000));

		play_end = true;
		});

	std::thread t2([&]() {
		auto playerc = player.Create(Track(1), Patch(0));
		playerc.Sleep(DeTime(500));

		while (!play_end) {
			//playerc.PlayPitch(Pitch(Note(NoteBase(5)), Octave(3)), DeTime(500));
		}
		});

	t1.join();
	t2.join();

	return 0;
}
