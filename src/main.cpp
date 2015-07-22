#include <Dunjun/Config.hpp>
#include <Dunjun/Game.hpp>

#undef main // NOTE(bill): SDL redefines main but we don't need it

int main(int argCount, char** args)
{
	Dunjun::Game::init(argCount, args);
	Dunjun::Game::run();
	// NOTE(bill): Only enable if you need to destroy everything
	// E.g. check for memory leaks else where in the code
	// Dunjun::Game::shutdown();

	return EXIT_SUCCESS;
}
