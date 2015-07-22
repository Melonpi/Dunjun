#include <Dunjun/Graphics/Texture.hpp>
#include <Dunjun/Graphics/Image.hpp>
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

Texture loadTextureFromFile(const String& filename,
                            TextureFilter minMagFilter,
                            TextureWrapMode wrapMode)
{
	Image image = loadImageFromFile(filename);
	defer(destroyImage(image));

	flipImageVertically(image);

	return loadTextureFromImage(image, minMagFilter, wrapMode);
}

Texture loadTextureFromImage(const Image& image,
                             TextureFilter minMagFilter,
                             TextureWrapMode wrapMode)
{
	if (image.format == ImageFormat::None)
		return Texture{};

	Texture tex = Texture{0, image.width, image.height};

	glGenTextures(1, &tex.handle);

	glEnable(GL_TEXTURE_2D);
	defer(glDisable(GL_TEXTURE_2D));

	glBindTexture(GL_TEXTURE_2D, tex.handle);
	defer(glBindTexture(GL_TEXTURE_2D, 0));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (s32)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (s32)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (s32)minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (s32)minMagFilter);

	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             getInteralFormat(image.format, true),
	             tex.width,
	             tex.height,
	             0,
	             getInteralFormat(image.format, false),
	             GL_UNSIGNED_BYTE,
	             image.pixels);

	return tex;
}

void destroyTexture(Texture& t)
{
	if (t.handle)
		glDeleteTextures(1, &t.handle);
}

void bindTexture(const Texture* tex, u32 position)
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
	// defer(glDisable(GL_TEXTURE_2D));

	if (tex && tex->handle)
		glBindTexture(GL_TEXTURE_2D, tex->handle);
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace Dunjun
