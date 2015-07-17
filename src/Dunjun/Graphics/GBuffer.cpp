#include <Dunjun/Graphics/GBuffer.hpp>

#include <Dunjun/Core/Array.hpp>
#include <Dunjun/Core/Memory.hpp>

namespace Dunjun
{
void destroyGBuffer(GBuffer& b)
{
	for (usize i = 0; i < GBuffer::Count; i++)
		destroyTexture(b.textures[i]);
	if (b.fbo)
		glDeleteFramebuffers(1, &b.fbo);
}

bool createGBuffer(GBuffer& b, u32 w, u32 h)
{
	if (w == b.width && h == b.height) // GBuffer already exists
		return true;

	b.width  = w;
	b.height = h;

	glGenFramebuffers(1, &b.fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, b.fbo);
	defer(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	glEnable(GL_TEXTURE_2D);
	defer(glBindTexture(GL_TEXTURE_2D, 0));

	Array<GLenum> drawBuffers{defaultAllocator()};

	auto addRT = [&drawBuffers, w, h](Texture& tex,
	                                  GLenum attachment,
	                                  s32 internalFormat,
	                                  GLenum format,
	                                  GLenum type)
	{
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(
		    GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex.handle, 0);

		if (attachment != GL_DEPTH_ATTACHMENT)
			append(drawBuffers, attachment);
	};

	addRT(b.textures[GBuffer::Diffuse],
	      GL_COLOR_ATTACHMENT0,
	      GL_RGB8,
	      GL_RGB,
	      GL_UNSIGNED_BYTE);
	addRT(b.textures[GBuffer::Specular],
	      GL_COLOR_ATTACHMENT1,
	      GL_RGBA8,
	      GL_RGBA,
	      GL_UNSIGNED_BYTE);
	addRT(b.textures[GBuffer::Normal],
	      GL_COLOR_ATTACHMENT2,
	      GL_RGB10_A2,
	      GL_RGBA,
	      GL_FLOAT);
	addRT(b.textures[GBuffer::Depth],
	      GL_DEPTH_ATTACHMENT,
	      GL_DEPTH_COMPONENT24,
	      GL_DEPTH_COMPONENT,
	      GL_FLOAT);

	glDrawBuffers((u32)len(drawBuffers), &drawBuffers[0]);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}

void bindGBuffer(const GBuffer* b)
{
	if (!b)
	{
		glFlush();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, b->fbo);
	}
}
} // namespace Dunjun
