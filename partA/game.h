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
		unsigned getNextRound();
		void updateZombies(unsigned int curr_round);
		void spawnZombies(unsigned int curr_round);
		void attackZombies(unsigned int curr_round);
		void printMedian(unsigned int curr_round);
		void printOutput(unsigned int curr_round);
		void printStats();

		std::vector<Zombie> zombie;
		std::vector<Zombie *> killed;
		std::priority_queue<Zombie *, std::vector<Zombie *>, ETAComparator> pq_eta;
		std::string killer_zombie = "";
		unsigned int quiver_cap = 0;
		unsigned int stats_num = 0;
		bool is_player_alive = true;
		bool verbose = false;
		bool median = false;
		/* end of private members */

};
