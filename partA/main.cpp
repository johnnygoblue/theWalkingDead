// IDENTIFIER  = 9504853406CBAC39EE89AA3AD238AA12CA198043
#include "game.h"

int main(int argc, char **argv) {
	Game deadline = Game();
	deadline.getOptions(argc, argv);
	deadline.readHeader();
	deadline.startGame();
	return 0;
}
