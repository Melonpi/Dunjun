#include <Dunjun/Game.hpp>

#include <SDL/SDL_main.h>
#undef main

int main(int argCount, char** args)
{
	Dunjun::Game::init(argCount, args);
	Dunjun::Game::run();
	Dunjun::Game::shutdown();

	return EXIT_SUCCESS;
}
