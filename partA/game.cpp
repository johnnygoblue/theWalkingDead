#include "game.h"

#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <string>

using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::string;

void Game::get_options(int argc, char **argv) {
	int option_index = 0, option = 0;
	opterr = false;

	struct option longOpts[] = {{ "verbose", no_argument, nullptr, 'v'},
								{ "statistics", required_argument, nullptr, 's'},
								{ "median", no_argument, nullptr, 'm'},
								{ "help", required_argument, nullptr, 'h'},
								{ nullptr, 0, nullptr, '\0'}};
    while ((option = getopt_long(argc, argv, "vs:mh", longOpts, &option_index)) != -1) {
		switch (option) {
			case 'v':
				verbose = true;
				break;
			case 's':
				stats_num  = unsigned(std::stol(optarg));
				break;
			case 'm':
				median = true;
				break;
			case 'h':
				cout << "EECS 281 Project 2 The Walking Deadline\n";
				cout << "Please refer to specs to simulate the game!\n";
				exit(0);
			default:
				cerr << "Unrecognized option '" << option << "'" << endl;
				exit(1);
		} // switch
	} // while
}

void Game::read_header() {
	string attr = "";
	string val = "";
	while (cin >> attr) {
		if (attr == "#") {
			getline(cin, val);
			continue;
		}
		if (attr == "quiver-capacity:") {
			cin >> val;
			quiver_cap = (unsigned)stol(val);
		} else if (attr == "random-seed:") {
			cin >> val;
			random_seed = (unsigned)stol(val);
		} else if (attr == "max-rand-distance:") {
			cin >> val;
			max_rand_dist = (unsigned)stol(val);
		} else if (attr == "max-rand-speed:") {
			cin >> val;
			max_rand_speed = (unsigned)stol(val);
		} else if (attr == "max-rand-health:") {
			cin >> val;
			max_rand_health = (unsigned)stol(val);
		} else if (attr == "---") {
			return;
		} else {
			cerr << "Unrecognized header '" << attr << "' in input!" << endl;
			exit(1);
		}
	} // while
}

void Game::dbg_read_header() {
	cout << "-----------------------------------\n";
	cout << "quiver-capicity = " << quiver_cap << endl;
	cout << "random-seed = " << random_seed << endl;
	cout << "max-rand-distance = " << max_rand_dist << endl;
	cout << "max-rand-speed = " << max_rand_speed << endl;
	cout << "max-rand-health = " << max_rand_health << endl;
	cout << "statistics number = " << stats_num << endl;
	cout << "print verbose = " << verbose << endl;
	cout << "print median = " << median << endl;
	cout << "-----------------------------------\n";
}
