#include "midiplayer.h"
#include <thread>
#include <vector>

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

	Player player;

	bool play_end = false;

	std::thread t1([&]() {
		auto playerc = player.Create(Track(0));
		playerc.PlayPitch(Note('C'), DeTime(500));
		playerc.PlayPitch(Note('C'), DeTime(500));
		playerc.PlayPitch(Note(3), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(1000));

		Pitch vecs[] { Note(1), Note(3), Note(5) };
		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(Note(5), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(1000));
		playerc.PlayPitch(Note(3), DeTime(500));
		playerc.PlayPitch(Note(3), DeTime(1000));
		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));

		playerc.PlayPitch(Note(1), DeTime(500));
		playerc.PlayPitch(Note(1), DeTime(500));
		playerc.PlayPitch(Note(3), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(1000));

		//playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(Note(5), DeTime(500));
		playerc.PlayPitch(Note(5), DeTime(1000));
		playerc.PlayPitch(Note(4), DeTime(500));
		playerc.PlayPitch(Note(4), DeTime(1000));

		play_end = true;
		});

	std::thread t2([&]() {
		auto playerc = player.Create(Track(1), Patch(0));
		playerc.Sleep(DeTime(500));
		
		while (!play_end) {
			playerc.PlayPitch(Pitch(Note(5), Octave(3)), DeTime(500));
		}
		});

	t1.join();
	t2.join();

	return 0;
}
