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
		/* internal functions */
		unsigned int getNextRound();
		void updateZombies();
		void spawnZombies(unsigned int curr_round);
		void attackZombies(unsigned int curr_round);
		void updateRemainingZombies(unsigned int curr_round);
		void printMedian(unsigned int curr_round);
		void printOutput(unsigned int curr_round);
		void printStatistics();

		/* debug functions */
		void dbg_print_pq_eta(unsigned round);
		void dbg_print_zombie_deque();

		/* member variables */
		std::deque<Zombie> zombie;
		std::vector<Zombie *> killed;
		std::priority_queue<Zombie *, std::deque<Zombie *>, Zombie::ETAComparator> pq_eta;
		std::string killer_zombie = "";
		unsigned int quiver_cap = 0;
		unsigned int stats_num = 0;
		bool is_player_alive = true;
		bool dead_walking = false;
		bool verbose = false;
		bool median = false;
		/* end of private members */

};
