#include <Dunjun/Graphics/GBuffer.hpp>

#include <vector>

namespace Dunjun
{
void GBuffer::destroy()
{
	if (fbo)
		glDeleteFramebuffersEXT(1, &fbo);
}

bool GBuffer::create(u32 w, u32 h)
{
	if (w == width && h == height) // GBuffer already exists
		return true;

	width  = w;
	height = h;

	if (!fbo)
		glGenFramebuffersEXT(1, &fbo);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	std::vector<GLenum> drawBuffers;

	auto addRT = [&drawBuffers, w, h](Texture& tex,
	                                  GLenum attachment,
	                                  s32 internalFormat,
	                                  GLenum format,
	                                  GLenum type)
	{
		if (!tex.handle)
			glGenTextures(1, &tex.handle);
		glBindTexture(GL_TEXTURE_2D, tex.handle);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             internalFormat,
		             (s32)w,
		             (s32)h,
		             0,
		             format,
		             type,
		             nullptr);
		tex.width  = w;
		tex.height = h;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, attachment, tex.handle, 0);

		if (attachment != GL_DEPTH_ATTACHMENT_EXT)
			drawBuffers.emplace_back(attachment);
	};

	addRT(textures[Diffuse],
	      GL_COLOR_ATTACHMENT0_EXT,
	      GL_RGB8,
	      GL_RGB,
	      GL_UNSIGNED_BYTE);
	addRT(textures[Specular],
	      GL_COLOR_ATTACHMENT1_EXT,
	      GL_RGBA8,
	      GL_RGBA,
	      GL_UNSIGNED_BYTE);
	addRT(textures[Normal],
	      GL_COLOR_ATTACHMENT2_EXT,
	      GL_RGB10_A2,
	      GL_RGBA,
	      GL_FLOAT);
	addRT(textures[Depth],
	      GL_DEPTH_ATTACHMENT_EXT,
	      GL_DEPTH_COMPONENT24,
	      GL_DEPTH_COMPONENT,
	      GL_FLOAT);

	glDrawBuffers((u32)len(drawBuffers), &drawBuffers[0]);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) !=
	    GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

void GBuffer::bind(const GBuffer* b)
{
	if (!b)
		glFlush();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, b != nullptr ? b->fbo : 0);
}
} // namespace Dunjun
