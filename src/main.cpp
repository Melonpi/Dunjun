#include <Dunjun/Game.hpp>

#ifndef SDL_main
#include <SDL/SDL_main.h>
#endif

int main(int argc, char** argv)
{
	Dunjun::Game::init(argc, argv);
	Dunjun::Game::run();
	Dunjun::Game::shutdown();

	return 0;
}
