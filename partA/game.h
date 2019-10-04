
class Game {
	public:
		Game() {}
		void get_options(int argc, char **argv);
		void read_header();
		void dbg_read_header();
	private:
		unsigned quiver_cap = 0;
		unsigned random_seed = 0;
		unsigned max_rand_dist = 0;
		unsigned max_rand_speed = 0;
		unsigned max_rand_health = 0;
		unsigned stats_num = 0;
		bool verbose = false;
		bool median = false;
};
