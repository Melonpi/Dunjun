#ifndef DUNJUN_GRAPHICS_IMAGE_HPP
#define DUNJUN_GRAPHICS_IMAGE_HPP

#include <Dunjun/Types.hpp>

#include <string>

namespace Dunjun
{
enum class ImageFormat
{
	None           = 0,
	Greyscale      = 1,
	GreyscaleAlpha = 2,
	RGB            = 3,
	RGBA           = 4,
};

struct Image
{
	ImageFormat format;
	u32 width;
	u32 height;
	u8* pixels;
};

Image loadImageFromFile(const std::string& filename);
Image loadImageFromMemory(u32 width,
                          u32 height,
                          ImageFormat format,
                          const u8* pixels);

void flipImageVertically(Image& image);

void destroyImage(Image& image);
} // namespace Dunjun

#endif
