// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
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
		unsigned int getNextRound();
		void updateZombies();
		void spawnZombies(unsigned int curr_round);
		void attackZombies(unsigned int curr_round);
		void printMedian(unsigned int curr_round);
		void printOutput(unsigned int curr_round);
		void printStatistics(unsigned int curr_round);

		// debug functions
		void dbg_print_pq_eta();
		void dbg_print_zombie_vector();

		std::vector<Zombie> zombie;
		std::vector<Zombie *> killed;
		std::priority_queue<Zombie *, std::vector<Zombie *>, Zombie::ETAComparator> pq_eta;
		std::string killer_zombie = "";
		unsigned int quiver_cap = 0;
		unsigned int stats_num = 0;
		bool is_player_alive = true;
		bool verbose = false;
		bool median = false;
		/* end of private members */

};
