#include "game.h"

int main(int argc, char **argv) {
	Game zombie = Game();
	zombie.get_options(argc, argv);
	zombie.read_header();
	zombie.dbg_read_header();
	return 0;
}
