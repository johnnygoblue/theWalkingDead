#include "game.h"

int main(int argc, char **argv) {
	Game deadline = Game();
	deadline.getOptions(argc, argv);
	deadline.readHeader();
	deadline.startGame();
	return 0;
}
