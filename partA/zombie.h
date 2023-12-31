// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
#include <string>
#include <queue>
#include <deque>
#include <iostream>

class Zombie {
	public:
		Zombie(const std::string name, const unsigned int dist, const unsigned int sp, const unsigned int hp, const unsigned int round) : name(name), distance(dist), speed(sp), health(hp), round_created(round), round_killed(0) {}
		unsigned getETA() const {
			if (speed == 0) {
				std::cerr << name << " ";
				std::cerr << "Division by 0!\n ";
			}
			return distance / speed;
		};
		unsigned getLifeTime() const {
			return round_killed - round_created + 1;
		}
		unsigned getLifeTime(unsigned int last_round) const {
			if (round_killed == 0) {
				return last_round - round_created + 1;
			}
			return getLifeTime();
		}
		std::string name;
		unsigned int distance;
		unsigned int speed;
		unsigned int health;
		unsigned int round_created;
		unsigned int round_killed;

		// Compare two pointer to Zombies by their ETA
		struct ETAComparator {
			bool operator()(const Zombie *left, const Zombie *right) const {
				if (left->getETA() > right->getETA()) {
					return true;
				} else if (left->getETA() < right->getETA()) {
					return false;
				} else { // same ETA
					if (left->health > right->health) {
						return true;
					} else if (left->health < right->health) {
						return false;
					} else {
						return right->name < left->name;
					}
				} // else same ETA
			} // operator
		}; // ETAComparator

		// Only killed container should use this comparator
		struct LifeTimeComparator {
			bool operator()(const Zombie *left, const Zombie *right) const {
				if (left->getLifeTime() > right->getLifeTime()) {
					return true;
				}
				return false;
			}
		}; // LifeTimeComparator

		// Only used in printing statistics
		struct MinLifeTimeComparator {
			bool operator()(const Zombie *left, const Zombie *right) const {
				if (left->getLifeTime() > right->getLifeTime()) {
					return true;
				} else if (left->getLifeTime() < right->getLifeTime()) {
					return false;
				} else {
					return left->name > right->name;
				}
			}
		}; // MinLifeTimeComparator

		// Only used in printing statistics
		struct MaxLifeTimeComparator {
			bool operator()(const Zombie *left, const Zombie *right) const {
				if (left->getLifeTime() < right->getLifeTime()) {
					return true;
				} else if (left->getLifeTime() > right->getLifeTime()) {
					return false;
				} else {
					return left->name > right->name;
				}
			}
		}; // MaxLifeTimeComparator

}; // Zombie

// Create this class instance when doing a total life sort on zombie deque
// this should only be called upon when printing statistics
class SortByLifeTimeMax {
	const std::deque<Zombie> &_z;
	unsigned int _last;

	public:
		SortByLifeTimeMax(const std::deque<Zombie> &z, unsigned int last_round) : _z(z), _last(last_round) {}

		bool operator()(unsigned int i, unsigned int j) const {
			if(_z[i].getLifeTime(_last) < _z[j].getLifeTime(_last)) {
				return true;
			} else if (_z[i].getLifeTime(_last) > _z[j].getLifeTime(_last)) {
				return false;
			} else {
				return _z[i].name < _z[j].name;
			}
		} //operator()()
};

class SortByLifeTimeMin {
	const std::deque<Zombie> &_z;
	unsigned int _last;

	public:
		SortByLifeTimeMin(const std::deque<Zombie> &z, unsigned int last_round) : _z(z), _last(last_round) {}

		bool operator()(unsigned int i, unsigned int j) const {
			if(_z[i].getLifeTime(_last) > _z[j].getLifeTime(_last)) {
				return true;
			} else if (_z[i].getLifeTime(_last) < _z[j].getLifeTime(_last)) {
				return false;
			} else {
				return _z[i].name < _z[j].name;
			}
		} //operator()()
};
