#include <Dunjun/Graphics/Image.hpp>

#include <Dunjun/Common.hpp>
#include <Dunjun/Core/Memory.hpp>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <cstring>

namespace Dunjun
{
Image loadImageFromFile(const String& filename)
{
	int w, h, f;
	u8* p = stbi_load(cString(filename), &w, &h, &f, 0);
	defer(stbi_image_free(p));

	if (!p)
	{
		panic(stbi_failure_reason());
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

	u32 imageSize = img.width * img.height * (u32)img.format;

	Allocator& a = defaultAllocator();

	if (img.pixels)
		a.deallocate(img.pixels);

	img.pixels = (u8*)a.allocate(imageSize * sizeof(u8));

	if (p != nullptr)
		memcpy(img.pixels, p, imageSize);

	return img;
}

void flipImageVertically(Image& img)
{
	Allocator& a = defaultAllocator();

	const u32 pitch    = img.width * (u32)img.format;
	const u32 halfRows = img.height / 2;

	u8* rowBuffer = (u8*)a.allocate(pitch * sizeof(u8));
	defer(a.deallocate(rowBuffer));

	for (u32 i = 0; i < halfRows; i++)
	{
		u8* row = img.pixels + (i * img.width) * (u32)img.format;
		u8* oppositeRow =
		    img.pixels + ((img.height - i - 1) * img.width) * (u32)img.format;

		memcpy(rowBuffer, row, pitch);
		memcpy(row, oppositeRow, pitch);
		memcpy(oppositeRow, rowBuffer, pitch);
	}
}

void destroyImage(Image& image) { defaultAllocator().deallocate(image.pixels); }

} // namespace Dunjun
