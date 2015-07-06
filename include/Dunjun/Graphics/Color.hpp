#ifndef DUNJUN_GRAPHICS_COLOR_HPP
#define DUNJUN_GRAPHICS_COLOR_HPP

#include <Dunjun/Common.hpp>

#include <type_traits>

namespace Dunjun
{
struct Color
{
	u8 r, g, b, a;

	GLOBAL const Color White;
	GLOBAL const Color Grey;
	GLOBAL const Color Black;
	GLOBAL const Color Brown;
	GLOBAL const Color Transparent;

	GLOBAL const Color Red;
	GLOBAL const Color Orange;
	GLOBAL const Color Yellow;
	GLOBAL const Color Green;
	GLOBAL const Color Cyan;
	GLOBAL const Color Blue;
	GLOBAL const Color Magenta;
};
}

#endif
