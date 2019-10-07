// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
#include <string>
#include <queue>

class Zombie {
	public:
		Zombie(const std::string name, const unsigned int dist, const unsigned int sp, const unsigned int hp, const unsigned int round) : name(name), distance(dist), speed(sp), health(hp), round_created(round), round_killed(0) {}
		unsigned getETA() const {
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
}; // Zombie

// Create this class instance when doing a total life sort on zombie vector
// this should only be called upon when printing statistics
class SortByLifeTimeMax {
	const std::vector<Zombie> &_z;
	unsigned int _last;

	public:
		SortByLifeTimeMax(const std::vector<Zombie> &z, unsigned int last_round) : _z(z), _last(last_round) {}

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
	const std::vector<Zombie> &_z;
	unsigned int _last;

	public:
		SortByLifeTimeMin(const std::vector<Zombie> &z, unsigned int last_round) : _z(z), _last(last_round) {}

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
