#include <string>
#include <queue>

class Zombie {
	public:
		Zombie(const std::string name, const unsigned dist, const unsigned sp, const unsigned hp, const unsigned round, const unsigned order) : name(name), distance(dist), speed(sp), health(hp), round_created(round), order_created(order), round_killed(0) {}
		unsigned getETA() const {
			return distance / speed;
		};
		std::string name;
		unsigned distance;
		unsigned speed;
		unsigned health;
		unsigned round_created;
		unsigned order_created;
		unsigned round_killed;
};

struct SortByCreation {
	bool operator()(const Zombie &left, const Zombie &right) const {
		if (left.round_created > right.round_created) {
			return true;
		} else if (left.round_created < right.round_created) {
			return false;
		} else { // created in same round
			return left.order_created > right.order_created;
		}
	}
};

struct SortByETA {
	bool operator()(const Zombie &left, const Zombie &right) const {
		if (left.getETA() > right.getETA()) {
			return true;
		} else if (left.getETA() > right.getETA()) {
			return false;
		} else { // same ETA
			if (left.health > right.health) {
				return true;
			} else if (left.health < right.health) {
				return false;
			} else {
				return right.name < left.name;
			}
		} // else same ETA
	} // operator
};
