#include <Dunjun/Game.hpp>

#include <SDL/SDL_main.h>
#undef main

int main(int argCount, char** args)
{
	Dunjun::Game::init(argCount, args);
	Dunjun::Game::run();
	// NOTE(bill): Only enable if you need to destroy everything
	// E.g. check for memory leaks else where in the code
	// Dunjun::Game::shutdown();

	return EXIT_SUCCESS;
}
