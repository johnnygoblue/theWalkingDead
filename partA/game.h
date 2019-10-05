#include "zombie.h"

#include <queue>
#include <string>
#include <vector>

class Game {
	public:
		Game() {}
		void getOptions(int argc, char **argv);
		void readHeader();
		void startGame();
		/* end of pubic members */

	private:
		unsigned getNewRound();
		void spawnZombies(unsigned round);

		std::priority_queue<Zombie *, std::vector<Zombie *>, SortByCreation()> pq_create;
		std::priority_queue<Zombie *, std::vector<Zombie *>, SortByETA()> pq_eta;
		unsigned quiver_cap = 0;
		unsigned stats_num = 0;
		bool verbose = false;
		bool median = false;
		/* end of private members */

};
