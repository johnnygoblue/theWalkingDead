#include "game.h"

int main(int argc, char **argv) {
	Game zombie = Game();
	zombie.getOptions(argc, argv);
	zombie.readHeader();
	//zombie.dbg_read_header();
	zombie.startGame();
	return 0;
}
