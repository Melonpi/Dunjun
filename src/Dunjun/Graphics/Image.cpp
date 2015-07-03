#include <Dunjun/Graphics/Image.hpp>

#include <Dunjun/Common.hpp>
#include <Dunjun/System/Memory.hpp>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <cstring>

namespace Dunjun
{
Image loadImageFromFile(const std::string& filename)
{
	int w, h, f;
	u8* p = stbi_load(filename.c_str(), &w, &h, &f, 0);
	defer(stbi_image_free(p));

	if (!p)
	{
		std::cerr << stbi_failure_reason() << std::endl;
		return {};
	}

	return loadImageFromMemory(w, h, (ImageFormat)f, p);
}

Image loadImageFromMemory(u32 w, u32 h, ImageFormat f, const u8* p)
{
	if (w == 0)
	{
		std::cerr << "Zero width image." << std::endl;
		return {};
	}
	if (h == 0)
	{
		std::cerr << "Zero height image." << std::endl;
		return {};
	}
	Image img = {};

	img.width  = w;
	img.height = h;
	img.format = f;

	usize imageSize = img.width * img.height * (usize)img.format;

	//Allocator& a = defaultAllocator();

	//if (img.pixels)
	//	a.deallocate(img.pixels);

	//img.pixels = (u8*)a.allocate(imageSize * sizeof(u8));
	if (img.pixels)
		delete[] img.pixels;

	img.pixels = new u8[imageSize];

	if (p != nullptr)
		memcpy(img.pixels, p, imageSize);

	return img;
}

void flipImageVertically(Image& img)
{
	Allocator& a = defaultAllocator();

	const usize pitch   = img.width * (usize)img.format;
	const u32 halfRows  = img.height / 2;

	//u8* rowBuffer = (u8*)a.allocate(pitch * sizeof(u8), alignof(u8));
	//defer(a.deallocate(rowBuffer));
	u8* rowBuffer = new u8[pitch];
	defer(delete[] rowBuffer);


	for (u32 i{0}; i < halfRows; i++)
	{
		u8* row = img.pixels + (i * img.width) * (usize)img.format;
		u8* oppositeRow =
		    img.pixels + ((img.height - i - 1) * img.width) * (usize)img.format;

		memcpy(rowBuffer, row, pitch);
		memcpy(row, oppositeRow, pitch);
		memcpy(oppositeRow, rowBuffer, pitch);
	}
}

void destroyImage(Image& image)
{
	delete[] image.pixels;
	//defaultAllocator().deallocate(image.pixels);
}

} // namespace Dunjun
