#include "GameErrors.h"
#include <iostream>
#include <SDL/SDL.h>
#include <cstdlib>

namespace JCEngine {
	void fatalError(std::string error) {
		std::cout << error << "\n";
		std::cout << "Enter any key to quit\n";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(69);
	}
}
