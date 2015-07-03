#include <Dunjun/Graphics/Image.hpp>
#include <Dunjun/Common.hpp>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <cstring>

namespace Dunjun
{
Image::Image(const Image& other)
: format{ImageFormat::None}
, width{0}
, height{0}
, pixels{nullptr}
{
	loadFromMemory(other.width, other.height, other.format, other.pixels);
}

Image& Image::operator=(const Image& other)
{
	loadFromMemory(other.width, other.height, other.format, other.pixels);
	return *this;
}

Image::~Image()
{
	if (pixels)
		delete[] pixels;
}

bool Image::loadFromFile(const std::string& filename)
{
	int w, h, f;
	u8* p = stbi_load(filename.c_str(), &w, &h, &f, 0);

	if (!p)
	{
		std::cerr << stbi_failure_reason() << std::endl;
		return false;
	}

	loadFromMemory(w, h, (ImageFormat)f, p);

	stbi_image_free(p);

	if (pixels)
		return true;
	return false;
}

bool Image::loadFromMemory(u32 w, u32 h, ImageFormat f, const u8* p)
{
	if (w == 0)
	{
		std::cerr << "Zero width image." << std::endl;
		return false;
	}
	if (h == 0)
	{
		std::cerr << "Zero height image." << std::endl;
		return false;
	}

	width  = w;
	height = h;
	format = f;

	usize imageSize = width * height * (usize)f;

	if (pixels)
		delete[] pixels;

	pixels = new u8[imageSize];

	if (p != nullptr)
		memcpy(pixels, p, imageSize);

	return true;
}

u8* Image::getPixel(u32 column, u32 row) const
{
	if (column >= width || row >= height)
		return nullptr;

	return pixels + (row * width + column) * (usize)format;
}

void Image::setPixel(u32 column, u32 row, const u32* pixel)
{
	if (column >= width || row >= height)
		return;

	u8* p = getPixel(column, row);
	memcpy(p, pixel, (usize)format);
}

void Image::flipVertically()
{
	usize pitch   = width * (usize)format;
	u32 halfRows  = height / 2;
	u8* rowBuffer = new u8[pitch];
	defer(delete[] rowBuffer);

	for (u32 i{0}; i < halfRows; i++)
	{
		u8* row         = pixels + (i * width) * (usize)format;
		u8* oppositeRow = pixels + ((height - i - 1) * width) * (usize)format;

		memcpy(rowBuffer, row, pitch);
		memcpy(row, oppositeRow, pitch);
		memcpy(oppositeRow, rowBuffer, pitch);
	}
}

// void Image::rotate90CCW()
//{

//}

// void Image::copyRectFromImage(const Image& src,
//							  u32 srcCol, u32 srcRow,
//							  u32 destCol, u32 destRow,
//							  u32 width, u32 height)
//{

//}
} // namespace Dunjun
