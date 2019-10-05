#include "game.h"
#include "zombie.h"
#include "P2random.h"

#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <string>

using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::string;

void Game::getOptions(int argc, char **argv) {
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

void Game::readHeader() {
	string attr = "";
	string val = "";
	unsigned rand_seed = 0;
	unsigned max_rand_dist = 0;
	unsigned max_rand_speed = 0;
	unsigned max_rand_hp = 0;

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
			rand_seed = (unsigned)stol(val);
		} else if (attr == "max-rand-distance:") {
			cin >> val;
			max_rand_dist = (unsigned)stol(val);
		} else if (attr == "max-rand-speed:") {
			cin >> val;
			max_rand_speed = (unsigned)stol(val);
		} else if (attr == "max-rand-health:") {
			cin >> val;
			max_rand_hp = (unsigned)stol(val);
		} else if (attr == "---") {
			return;
		} else {
			cerr << "Unrecognized header '" << attr << "' in input!" << endl;
			exit(1);
		}
	} // while
	P2random::initialize(rand_seed, max_rand_dist, max_rand_speed, max_rand_hp);
}

void Game::startGame() {
}

// Return new round from input, 0 if no new round available
unsigned Game::getNewRound() {
	string round;
	cin >> round;
	if (round == "round") {
		cin >> round;
		return (unsigned)stol(round);
	}
	return 0;
}

void Game::spawnZombies(unsigned round_num) {
	string attr = "";
	string val = "";
	unsigned order = 0;
	unsigned num_rand_zombies = 0;
	unsigned num_named_zombies = 0;

	cin >> attr;
	cin >> val;
	num_rand_zombies = (unsigned)stol(val);
	cin >> attr;
	cin >> val;
	num_named_zombies = (unsigned)stol(val);

	// spawn random zombies
	for (size_t i = 0; i < num_rand_zombies; ++i) {
		string name = P2random::getNextZombieName();
		unsigned distance = P2random::getNextZombieDistance();
		unsigned speed = P2random::getNextZombieSpeed();
		unsigned health = P2random::getNextZombieHealth();
		pq_create.push(&Zombie(name, distance, speed, health, round_num, order));
		++order;
	}
	// spawn named zombies
	for (size_t i = 0; i < num_named_zombies; ++i) {
		string name = "";
		unsigned distance = 0;
		unsigned speed = 0;
		unsigned health = 0;
		cin >> name;
		cin >> attr >> val;
		distance = (unsigned)stol(val);
		cin >> attr >> val;
		speed = (unsigned)stol(val);
		cin >> attr >> val;
		health = (unsigned)stol(val);
		pq_create.push(&Zombie(name, distance, speed, health, round_num, order));
		++order;
	}
}
