#include "midiplayer.h"
#include "notation.h"
#include "parse.h"
#include <thread>
#include <vector>

#ifndef NDEBUG
void Test();
#endif

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

#ifndef NDEBUG
	Test();
#endif

	Player player;

	bool play_end = false;
	std::thread t1([&]() {
		auto playerc = player.Create(Track(0), Patch(0));

		Pitch base(Note("C"));

		Notation not1(NotationBase(1, false), NotationOctave(0));
		Notation not2(NotationBase(2, false), NotationOctave(0));
		Notation not3(NotationBase(3, false), NotationOctave(0));
		Notation not4(NotationBase(4, false), NotationOctave(0));
		Notation not5(NotationBase(5, false), NotationOctave(0));

		playerc.PlayPitch(not1 + base, DeTime(500));
		playerc.PlayPitch(not1 + base, DeTime(500));
		playerc.PlayPitch(not3 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(1000));

		playerc.PlayPitch(not5 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(1000));
		playerc.PlayPitch(not3 + base, DeTime(500));
		playerc.PlayPitch(not3 + base, DeTime(1000));

		playerc.PlayPitch(not1 + base, DeTime(500));
		playerc.PlayPitch(not1 + base, DeTime(500));
		playerc.PlayPitch(not3 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(1000));

		playerc.PlayPitch(not5 + base, DeTime(500));
		playerc.PlayPitch(not5 + base, DeTime(1000));
		playerc.PlayPitch(not4 + base, DeTime(500));
		playerc.PlayPitch(not4 + base, DeTime(1000));

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
