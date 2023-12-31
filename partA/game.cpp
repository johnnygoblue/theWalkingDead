// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
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
				stats_num  = unsigned(std::stoul(optarg));
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
			quiver_cap = (unsigned int)stoul(val);
		} else if (attr == "random-seed:") {
			cin >> val;
			rand_seed = (unsigned int)stoul(val);
		} else if (attr == "max-rand-distance:") {
			cin >> val;
			max_rand_dist = (unsigned int)stoul(val);
		} else if (attr == "max-rand-speed:") {
			cin >> val;
			max_rand_speed = (unsigned int)stoul(val);
		} else if (attr == "max-rand-health:") {
			cin >> val;
			max_rand_hp = (unsigned int)stoul(val);
		} else if (attr == "---") {
			P2random::initialize(rand_seed, max_rand_dist, max_rand_speed, max_rand_hp);
			return;
		} else {
			cerr << "Unrecognized header '" << attr << "' in input!" << endl;
			exit(1);
		}
	} // while
}

// main game loop
void Game::startGame() {
	unsigned int curr_round = 1; // current round is the round game currently in
	unsigned int next_round = 0; // next round is the round next up from input

	next_round = getNextRound();

	while (is_player_alive) {

		if (dead_walking && pq_eta.empty() && next_round == 0) {
			break; // victory exit
		}

		if (verbose) {
			cout << "Round: " << curr_round << "\n";
		}

		updateZombies();

		if (!is_player_alive) {
			if (stats_num) {
				updateRemainingZombies(curr_round);
			}
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

	if (stats_num > 0) {
		printStatistics();
	}
}

// Return new round from input, 0 if no new round available
unsigned int Game::getNextRound() {
	string round;
	cin >> round;
	if (round == "round:") {
		cin >> round;
		return (unsigned int)stoul(round);
	}
	return 0;
}

// update zombies according to spec
void Game::updateZombies() {
	for (size_t i = 0; i < zombie.size(); ++i) {
		// do not update dead zombies
		if (zombie[i].round_killed != 0) {
			continue;
		}
		if (zombie[i].distance < zombie[i].speed) {
			zombie[i].distance = 0;
		} else {
			zombie[i].distance -= zombie[i].speed;
		}
		//zombie[i].distance = zombie[i].distance > zombie[i].speed ? zombie[i].distance - zombie[i].speed : 0;
		if (verbose) {
			cout << "Moved: " << zombie[i].name << " (distance: " << zombie[i].distance <<
				", speed: " << zombie[i].speed << ", health: " << zombie[i].health << ")\n";
		}
		if (zombie[i].distance == 0) {
			// first zombie eats your brains!
			if (is_player_alive) {
				killer_zombie = zombie[i].name;
			}
			is_player_alive = false;
		}
	} // for
}

// Given a round_num that is previously read from input
// read all other input data pertaining to this round
void Game::spawnZombies(unsigned int curr_round) {
	string attr = "";
	string val = "";
	Zombie *z_ptr = nullptr;
	unsigned int num_rand_zombies = 0;
	unsigned int num_named_zombies = 0;

	cin >> attr;
	cin >> val;
	num_rand_zombies = (unsigned int)stoul(val);
	cin >> attr;
	cin >> val;
	num_named_zombies = (unsigned int)stoul(val);

	// if any zombie has been generated in game
	if (num_rand_zombies + num_named_zombies > 0) {
		dead_walking = true;
	}

	// spawn random zombies
	for (unsigned int i = 0; i < num_rand_zombies; ++i) {
		string name = P2random::getNextZombieName();
		unsigned int distance = P2random::getNextZombieDistance();
		unsigned int speed = P2random::getNextZombieSpeed();
		unsigned int health = P2random::getNextZombieHealth();
		Zombie rand = Zombie(name, distance, speed, health, curr_round);
		if (verbose) {
			cout << "Created: " << rand.name << " (distance: " <<
				rand.distance << ", speed: " << rand.speed << ", health: " <<
				rand.health << ")\n";
		}
		zombie.push_back(rand);
	}
	// spawn named zombies
	for (unsigned int i = 0; i < num_named_zombies; ++i) {
		string name = "";
		unsigned int distance = 0;
		unsigned int speed = 0;
		unsigned int health = 0;
		cin >> name;
		cin >> attr >> val;
		distance = (unsigned int)stoul(val);
		cin >> attr >> val;
		speed = (unsigned int)stoul(val);
		cin >> attr >> val;
		health = (unsigned int)stoul(val);
		Zombie named = Zombie(name, distance, speed, health, curr_round);
		if (verbose) {
			cout << "Created: " << named.name << " (distance: " <<
				named.distance << ", speed: " << named.speed << ", health: " <<
				named.health << ")\n";
		}
		zombie.push_back(named);
	}

	// push the spawned zombie pointers to priority queue
	for (unsigned long i = zombie.size() - 1, cnt = 0;
			cnt < num_rand_zombies + num_named_zombies; --i, ++cnt) {
		z_ptr = &zombie[i];
		pq_eta.push(z_ptr);
	}

	// consume "---" below
	cin >> attr;
}

void Game::attackZombies(unsigned int curr_round) {
	unsigned int arrow = quiver_cap; // number of arrows player currently has
	Zombie *z = nullptr;

	// attack when there is arrow in hand and zombie in sight
	while (arrow != 0 && !pq_eta.empty()) {
		z = pq_eta.top();
		if (arrow >= z->health) {
			arrow -= z->health;
			z->health = 0;
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
			arrow = 0;
		}
	} // while
}

// Update round_killed on zombies that the player did not killed
// Only call this function when player is eaten and print stats
// are required
void Game::updateRemainingZombies(unsigned int curr_round) {
	for (size_t i = 0; i < zombie.size(); ++i) {
		if (zombie[i].round_killed == 0) {
			zombie[i].round_killed = curr_round;
		}
	}
}

// Print median life time of zombies killed
void Game::printMedian(unsigned int curr_round) {
	if (!killed.empty()) {
		std::vector<Zombie *> killed_cpy = killed;
		unsigned int body_count = (unsigned int)killed_cpy.size();
		unsigned int median = 0;

		std::sort(killed_cpy.begin(), killed_cpy.end(), Zombie::LifeTimeComparator());
		if (body_count % 2 == 0) {
			median = (killed_cpy[body_count / 2 - 1]->getLifeTime() +
					killed_cpy[body_count / 2]->getLifeTime()) / 2;
		} else {
			median = killed_cpy[body_count / 2]->getLifeTime();
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

void Game::printStatistics() {
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
	cout << "Last zombies killed:\n"; // cnt should be min(cnt, killed.size()
	size_t cnt = stats_num < killed.size() ? stats_num : killed.size();
	for (size_t i = killed.size() - 1; cnt != 0; --i, --cnt) {
		cout << killed[i]->name << " " << cnt << "\n";
		if (i == 0) {
			break;
		}
	}

	// create deque of zombie pointers for PQ initialization
	std::deque<Zombie *> zombie_ptr(zombie.size(), nullptr);
	for (size_t i = 0; i < zombie.size(); ++i) {
		zombie_ptr[i] = &zombie[i];
	}

	// create PQs to hold zombie pointer based on total life time
	std::priority_queue<Zombie *, std::deque<Zombie *>, Zombie::MaxLifeTimeComparator> pqMax(zombie_ptr.begin(), zombie_ptr.end());

	// names of the N zombies who were active for the most number of rounds
	cout << "Most active zombies:\n";
	size_t num = stats_num < zombie.size() ? stats_num : zombie.size();
	for (size_t i = 0; i < num; ++i) {
		Zombie *z_ptr = pqMax.top();
		cout << z_ptr->name << " " << z_ptr->getLifeTime() << "\n";
		pqMax.pop();
	}

	std::priority_queue<Zombie *, std::deque<Zombie *>, Zombie::MinLifeTimeComparator> pqMin(zombie_ptr.begin(), zombie_ptr.end());

	// names of the N zombies who were active for the least number of rounds
	cout << "Least active zombies:\n";
	num = stats_num < zombie.size() ? stats_num : zombie.size();
	for (size_t i = 0; i < num; ++i) {
		Zombie *z_ptr = pqMin.top();
		cout << z_ptr->name << " " << z_ptr->getLifeTime() << "\n";
		pqMin.pop();
	}

	//// index sort all zombies ever created by total life time
	//std::vector<unsigned> idx(zombie.size(), 0);
	//for (unsigned i = 0; i < zombie.size(); ++i) {
	//	idx[i] = i;
	//}
	//SortByLifeTimeMin sbltn(zombie, curr_round);
	//sort(begin(idx), end(idx), sbltn);

	//// names of the N zombies who were active for the most number of rounds
	//cout << "Most active zombies:\n";
	//for (unsigned int i = 0; i < idx.size(); ++i) {
	//	if (i == stats_num) {
	//		break;
	//	}
	//	cout << zombie[idx[i]].name << " " << zombie[idx[i]].getLifeTime(curr_round) << "\n";
	//}

	//SortByLifeTimeMax sbltx(zombie, curr_round);
	//sort(begin(idx), end(idx), sbltx);
	//// names of the N zombies who were active for the least number of rounds
	//cout << "Least active zombies:\n";
	//for (unsigned int i = 0; i < idx.size(); ++i) {
	//	if (i == stats_num) {
	//		break;
	//	}
	//	cout << zombie[idx[i]].name << " " << zombie[idx[i]].getLifeTime(curr_round) << "\n";
	//}
}

void Game::dbg_print_pq_eta(unsigned round) {
	std::deque<Zombie *> tmp;

	cout << "-------------------------------------------------------------\n";
	cout << "Printing PQ ETA in round " << round << ":\n";
	while (!pq_eta.empty()) {
		Zombie *z = pq_eta.top();
		tmp.push_back(z);
		cout << z->name << " (distance: " << z->distance << ", speed: " << z->speed << ", health: " << z->health << ")\n";
		pq_eta.pop();
	}
	while (!tmp.empty()) {
		pq_eta.push(tmp.back());
		tmp.pop_back();
	}
	cout << "-------------------------------------------------------------\n";
}

void Game::dbg_print_zombie_deque() {
	cout << "-------------------------------------------------------------\n";
	cout << "Printing Zombie Deque:\n";
	for (const auto &z : zombie) {
		cout << z.name << " (distance: " << z.distance << ", speed: " << z.speed << ", health: " << z.health << ")\n";
	}
	cout << "-------------------------------------------------------------\n";
}
