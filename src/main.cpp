#include <Dunjun/Game.hpp>

int main(int argc, char** argv)
{
	Dunjun::Game::init();
	Dunjun::Game::run();
	Dunjun::Game::shutdown();

	return 0;
}
