#ifndef DUNJUN_GAME_HPP
#define DUNJUN_GAME_HPP

#include <Dunjun/Math.hpp>

namespace Dunjun
{
namespace Game
{
void init(int argCount, char** args);
void run();
void shutdown();

void glInit();
} // namespace Game
} // namespace Dunjun

#endif
