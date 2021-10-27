#include <Windows.h>
#include "MainGame.h"

int main(int argc, char** argv) {

	MainGame maingame(1024, 768);
	maingame.run();

	return 0;
}