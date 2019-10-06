#include "game.h"
#include "P2random.h"

#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <algorithm>

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

// Read all game info prior to first round information
void Game::readHeader() {
	string attr = "";
	string val = "";
	unsigned int rand_seed = 0;
	unsigned int max_rand_dist = 0;
	unsigned int max_rand_speed = 0;
	unsigned int max_rand_hp = 0;

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

// main game loop
void Game::startGame() {
	unsigned int curr_round = 0; // current round is the round game currently in
	unsigned int next_round = 0; // next round is the round next up from input

	next_round = getNextRound();
	while (is_player_alive) {
		if (curr_round != 0 && pq_eta.empty() && curr_round != next_round) {
			break; // victory exit
		}

		if (verbose) {
			cout << "Round " << curr_round << "\n";
		}

		updateZombies();

		if (!is_player_alive) {
			break; // defeat exit
		}

		if (curr_round == next_round) {
			spawnZombies(curr_round);
			next_round = getNextRound();
		}

		attackZombies(curr_round);

		if (median) {
			printMedian(curr_round);
		}

		++curr_round;
	} // while

	printOutput(curr_round);

	printStatistics(curr_round);
}

// Return new round from input, 0 if no new round available
unsigned int Game::getNextRound() {
	string round;
	cin >> round;
	if (round == "round") {
		cin >> round;
		return (unsigned int)stol(round);
	}
	cerr << "No next round!\n" << endl;
	return 0;
}

// update zombies according to spec
void Game::updateZombies() {
	for (auto z : zombie) {
		if (z.round_killed != 0) {
			continue;
		}
		z.distance = (unsigned int)std::max((int64_t)0, (int64_t)(z.distance - z.speed));
		if (verbose) {
			cout << "Moved " << z.name << " (distance: " << z.distance <<
				", speed: " << z.speed << ", health: " << z.health << ")\n";
		}
		if (z.distance == 0) {
			is_player_alive = false;
			killer_zombie = z.name;
		}
	} // for
}

// Given a round_num that is previously read from input
// read all other input data pertaining to this round
void Game::spawnZombies(unsigned int curr_round) {
	string attr = "";
	string val = "";
	unsigned int num_rand_zombies = 0;
	unsigned int num_named_zombies = 0;

	cin >> attr;
	cin >> val;
	num_rand_zombies = (unsigned int)stol(val);
	cin >> attr;
	cin >> val;
	num_named_zombies = (unsigned int)stol(val);

	// spawn random zombies
	for (unsigned int i = 0; i < num_rand_zombies; ++i) {
		string name = P2random::getNextZombieName();
		unsigned int distance = P2random::getNextZombieDistance();
		unsigned int speed = P2random::getNextZombieSpeed();
		unsigned int health = P2random::getNextZombieHealth();
		Zombie rand = Zombie(name, distance, speed, health, curr_round);
		if (verbose) {
			cout << "Created: " << rand.name << " (distance: " <<
				rand.distance << ", speed: " << rand.speed << ", health = " <<
				rand.health << ")\n";
		}
		zombie.push_back(rand);
		pq_eta.push(&rand);
	}
	// spawn named zombies
	for (unsigned int i = 0; i < num_named_zombies; ++i) {
		string name = "";
		unsigned int distance = 0;
		unsigned int speed = 0;
		unsigned int health = 0;
		cin >> name;
		cin >> attr >> val;
		distance = (unsigned int)stol(val);
		cin >> attr >> val;
		speed = (unsigned int)stol(val);
		cin >> attr >> val;
		health = (unsigned int)stol(val);
		Zombie named = Zombie(name, distance, speed, health, curr_round);
		if (verbose) {
			cout << "Created: " << named.name << " (distance: " <<
				named.distance << ", speed: " << named.speed << ", health = " <<
				named.health << ")\n";
		}
		zombie.push_back(named);
		pq_eta.push(&named);
	}
}

void Game::attackZombies(unsigned int curr_round) {
	unsigned int arrow = quiver_cap; // number of arrows player currently has
	Zombie *z = nullptr;

	while (arrow != 0 && !pq_eta.empty()) {
		z = pq_eta.top();
		if (arrow >= z->health) {
			arrow -= z->health;
			z->round_killed = curr_round;
			if (verbose) {
				cout << "Destroyed: " << z->name << " (distance: " <<
					z->distance << ", speed: " << z->speed << ", health: " <<
					z->health << ")\n";
			}
			killed.push_back(z);
			pq_eta.pop();
		} else {
			z->health = z->health - arrow;
		}
	} // while
}

// Print median life time of zombies killed
void Game::printMedian(unsigned int curr_round) {
	if (!killed.empty()) {
		unsigned int body_count = killed.size();
		float median = 0.0;

		std::sort(killed.begin(), killed.end(), LifeTimeComparator());
		if (body_count % 2 == 0) {
			median = (killed[body_count / 2 - 1]->getLifeTime() +
					killed[body_count / 2]->getLifeTime) / 2;
		} else {
			median = killed[body_count / 2]->getLifeTime();
		}
		cout << "At the end of round " << curr_round << ", the median zombie lifetime is " <<
			median << "\n";
	}
}

// Simply print infomation about VICTORY/DEFEAT
void Game::printOutput(unsigned int curr_round) {
	if (is_player_alive) {
		Zombie *z = killed.back();
		cout << "VICTORY IN ROUND " << curr_round - 1 << "! " << z->name <<
			" was the last zombie.\n";
	} else {
		cout << "DEFEAT IN ROUND " << curr_round << "! " << killer_zombie <<
			" ate your brains!\n";
	}
}

void Game::printStatistics(unsigned curr_round) {
	// number of zombies still active at the end
	cout << "Zombies still active: " << pq_eta.size() << "\n";

	// names of the first N zombies that were killed
	cout << "First zombies killed:\n";
	for (unsigned int i = 0; i < killed.size(); ++i) {
		if (i == stats_num) {
			break;
		}
		cout << killed[i]->name << " " << i + 1 << "\n";
	}

	// names of the last N zombies that were killed
	cout << "Last zombies killed:\n";
	for (int i = killed.size() - 1, cnt = stats_num; i >= 0; --i, --cnt) {
		if (cnt == 0) {
			break;
		}
		cout << killed[(unsigned int)i]->name << " " << cnt << "\n";
	}

	// index sort all zombies ever created by total life time
	vector<unsigned> idx = resize(zombie.size());
	for (unsigned int i = 0; i < zombie.size(); ++i) {
		idx[i] = i;
	}
	SortByLifeTime sblt(&zombie, curr_round);
	sort(begin(idx), zombie.end(idx), sblt);

	// names of the N zombies who were active for the most number of rounds
	cout << "Most active zombies:\n";
	for (int i = idx.size() - 1; i >= 0; --i) {
		if (idx.size() - i > stats_num) {
			break;
		}
		cout << zombie[idx[i]] << " " << zombie[idx[i]].getLifeTime(curr_round) << "\n";
	}

	// names of the N zombies who were active for the least number of rounds
	cout << "Least active zombies:\n";
	for (unsigned int i = 0; i < idx.size(); ++i) {
		if (i == stats_num) {
			break;
		}
		cout << zombies[idx[i]].name << " " << zombies[idx[i]].getLifeTime(curr_round) << "\n";
	}
}
