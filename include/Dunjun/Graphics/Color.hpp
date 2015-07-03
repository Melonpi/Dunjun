#ifndef DUNJUN_GRAPHICS_COLOR_HPP
#define DUNJUN_GRAPHICS_COLOR_HPP

#include <Dunjun/Common.hpp>

#include <type_traits>

namespace Dunjun
{
struct Color
{
	u8 r, g, b, a;

	GLOBAL const u32 Depth{256};
};
}

#endif
