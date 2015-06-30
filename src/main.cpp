#include <Dunjun/Game.hpp>

#include <SDL/SDL_main.h>
#undef main

int main(int argc, char** argv)
{
	Dunjun::Game::init(argc, argv);
	Dunjun::Game::run();
	Dunjun::Game::shutdown();

	return 0;
}
