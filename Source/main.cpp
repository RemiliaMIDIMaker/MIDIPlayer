#include "midiplayer.h"
#include "parse.h"
#include <thread>
#include <vector>

void Test() {
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

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

	Player player;

	bool play_end = false;

	std::thread t1([&]() {
		auto playerc = player.Create(Track(0));
		playerc.PlayPitch(Note("C"), DeTime(500));
		playerc.PlayPitch(Note(NoteBase('C')), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(3)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(1000));

		Pitch vecs[] { Note(NoteBase(1)), Note(NoteBase(3)), Note(NoteBase(5)) };
		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(1000));
		playerc.PlayPitch(Note(NoteBase(3)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(3)), DeTime(1000));
		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));

		playerc.PlayPitch(Note(NoteBase(1)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(1)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(3)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(1000));

		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(5)), DeTime(1000));
		playerc.PlayPitch(Note(NoteBase(4)), DeTime(500));
		playerc.PlayPitch(Note(NoteBase(4)), DeTime(1000));

		play_end = true;
		});

	std::thread t2([&]() {
		auto playerc = player.Create(Track(1), Patch(0));
		playerc.Sleep(DeTime(500));

		while (!play_end) {
			playerc.PlayPitch(Pitch(Note(NoteBase(5)), Octave(3)), DeTime(500));
		}
		});

	t1.join();
	t2.join();

	return 0;
}
