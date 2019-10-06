#include <string>
#include <queue>

class Zombie {
	public:
		Zombie(const std::string name, const unsigned int dist, const unsigned int sp, const unsigned int hp, const unsigned int round) : name(name), distance(dist), speed(sp), health(hp), round_created(round), round_killed(0) {}
		unsigned getETA() const {
			return distance / speed;
		};
		int getLifeTime() const {
			return round_killed - round_created;
		}
		std::string name;
		unsigned int distance;
		unsigned int speed;
		unsigned int health;
		unsigned int round_created;
		unsigned int round_killed;
};

struct ETAComparator {
	bool operator()(const Zombie *left, const Zombie *right) const {
		if (left->getETA() > right->getETA()) {
			return true;
		} else if (left->getETA() > right->getETA()) {
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
};

struct LifeTimeComparator {
	bool operator()(const Zombie *left, const Zombie *right) const {
		if (left->getLifeTime() > right->getLifeTime()) {
			return true;
		}
		return false;
}
