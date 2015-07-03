#include <Dunjun/Graphics/Texture.hpp>

#include <Dunjun/Common.hpp>

#include <cassert>

namespace Dunjun
{
INTERNAL GLenum getInteralFormat(ImageFormat format, bool srgb)
{
	switch (format)
	{
	case ImageFormat::Greyscale:
		return GL_LUMINANCE;
	case ImageFormat::GreyscaleAlpha:
		return GL_LUMINANCE_ALPHA;
	case ImageFormat::RGB:
		return (srgb ? GL_SRGB : GL_RGB);
	case ImageFormat::RGBA:
		return (srgb ? GL_SRGB_ALPHA : GL_RGBA);

	default:
	case ImageFormat::None:
		panic("Non-valid ImageFormat.");
		return 0;
	}
}

Texture::Texture(const Image& image,
                 TextureFilter minMagFilter,
                 TextureWrapMode wrapMode)
: handle{0}
, width{(s32)image.width}
, height{(s32)image.height}
{
	if (!loadFromImage(image, minMagFilter, wrapMode))
		panic("Could not create texture from image.");
}

bool Texture::loadFromFile(const std::string& filename,
                           TextureFilter minMagFilter,
                           TextureWrapMode wrapMode)
{
	Image image;
	if (!image.loadFromFile(filename))
		return false;
	image.flipVertically();

	return loadFromImage(image, minMagFilter, wrapMode);
}

bool Texture::loadFromImage(const Image& image,
                            TextureFilter minMagFilter,
                            TextureWrapMode wrapMode)
{
	if (image.format == ImageFormat::None)
		return false;

	width  = image.width;
	height = image.height;

	if (!handle)
		glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(
	    GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<s32>(wrapMode));
	glTexParameteri(
	    GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<s32>(wrapMode));
	glTexParameteri(
	    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<s32>(minMagFilter));
	glTexParameteri(
	    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<s32>(minMagFilter));

	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             getInteralFormat(image.format, true),
	             width,
	             height,
	             0,
	             getInteralFormat(image.format, false),
	             GL_UNSIGNED_BYTE,
	             image.pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

Texture::~Texture()
{
	if (handle)
		glDeleteTextures(1, &handle);
}

void Texture::bind(const Texture* tex, u32 position)
{
	if (position > 31)
	{
		std::cerr << "Textures can only be bount to position [0 ... 31]\n";
		std::cerr << "Will bind to position [31]" << std::endl;
		position = 31;
	}

	glActiveTexture(GL_TEXTURE0 + position);
	glClientActiveTexture(GL_TEXTURE0 + position);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (tex && tex->handle) ? tex->handle : 0);
	glDisable(GL_TEXTURE_2D);
}
} // namespace Dunjun
