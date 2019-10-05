#include <queue>
#include <string>

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

		priority_queue<Zombie *, vector<Zombie *>, SortByCreation()> pq_create;
		priority_queue<Zombie *, vector<Zombie *>, SortByETA()> pq_eta;
		unsigned quiver_cap = 0;
		unsigned stats_num = 0;
		bool verbose = false;
		bool median = false;
		/* end of private members */

};
