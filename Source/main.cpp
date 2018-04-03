#include "midiplayer.h"
#include <thread>
#include <vector>

int main(int argc, const char *argv[])
{
	using namespace MIDIPlayer;

	Player player;

	bool play_end = false;

	std::thread t1([&]() {
		auto playerc = player.Create(Channel(0));
		playerc.PlayPitch(PitchBase(1), DeTime(500));
		playerc.PlayPitch(PitchBase(1), DeTime(500));
		playerc.PlayPitch(PitchBase(3), DeTime(500));
		playerc.PlayPitch(PitchBase(5), DeTime(500));

		Pitch vecs[] { PitchBase(1), PitchBase(3), PitchBase(5) };
		playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(PitchBase(5), DeTime(500));
		playerc.PlayPitch(PitchBase(5), DeTime(1000));
		playerc.PlayPitch(PitchBase(3), DeTime(500));
		playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));

		playerc.PlayPitch(PitchBase(1), DeTime(500));
		playerc.PlayPitch(PitchBase(1), DeTime(500));
		playerc.PlayPitch(PitchBase(3), DeTime(500));
		playerc.PlayPitch(PitchBase(5), DeTime(500));

		playerc.PlayPitchs(std::begin(vecs), std::end(vecs), DeTime(1000));
		playerc.PlayPitch(PitchBase(5), DeTime(500));
		playerc.PlayPitch(PitchBase(5), DeTime(1000));
		playerc.PlayPitch(PitchBase(4), DeTime(500));
		playerc.PlayPitch(PitchBase(4), DeTime(1000));

		play_end = true;
		});

	std::thread t2([&]() {
		auto playerc = player.Create(Channel(1));
		playerc.Sleep(DeTime(500));
		
		while (!play_end) {
			playerc.PlayPitch(Pitch(PitchBase(5), Scale(3)), DeTime(500));
		}
		});

	t1.join();
	t2.join();

	return 0;
}
