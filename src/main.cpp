#include <Dunjun/Game.hpp>

#include <type_traits>

int main(int argc, char** argv)
{
	Dunjun::Game::init();
	Dunjun::Game::run();
	Dunjun::Game::cleanup();

	return 0;
}
