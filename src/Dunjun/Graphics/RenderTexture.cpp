#include <Dunjun/Graphics/RenderTexture.hpp>

#include <vector>

namespace Dunjun
{
RenderTexture::~RenderTexture()
{
	if (fbo)
		glDeleteFramebuffersEXT(1, &fbo);
}

bool RenderTexture::create(u32 w,
                           u32 h,
                           TextureType t,
                           TextureFilter minMagFilter,
                           TextureWrapMode wrapMode)
{
	if (w == width && h == height && t == type)
		return true;

	type   = t;
	width  = w;
	height = h;

	if (!fbo)
		glGenFramebuffersEXT(1, &fbo);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	if (type & TextureType::Color)
	{
		if (!colorTexture.handle)
			glGenTextures(1, &colorTexture.handle);
		glBindTexture(GL_TEXTURE_2D, (u32)colorTexture.handle);
		if (type & TextureType::Lighting)
		{
			glTexImage2D(GL_TEXTURE_2D,
			             0,
			             GL_RGB10_A2, // TODO(bill): decide upon lighting
			                          // resolution for each component
			             // 8b/c too small
			             // 32b/c too large
			             (s32)width,
			             (s32)height,
			             0,
			             GL_RGB,
			             GL_FLOAT,
			             0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
			             0,
			             GL_RGB,
			             (s32)width,
			             (s32)height,
			             0,
			             GL_RGB,
			             GL_UNSIGNED_BYTE,
			             0);
		}
		colorTexture.width  = width;
		colorTexture.height = height;

		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

		glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
		                        GL_COLOR_ATTACHMENT0_EXT,
		                        colorTexture.handle,
		                        0);
	}

	if (type & TextureType::Depth)
	{
		if (!depthTexture.handle)
			glGenTextures(1, &depthTexture.handle);
		glBindTexture(GL_TEXTURE_2D, (u32)depthTexture.handle);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             GL_DEPTH_COMPONENT24,
		             (s32)width,
		             (s32)height,
		             0,
		             GL_DEPTH_COMPONENT,
		             GL_FLOAT,
		             0);
		depthTexture.width  = width;
		depthTexture.height = height;

		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

		glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
		                        GL_DEPTH_ATTACHMENT_EXT,
		                        depthTexture.handle,
		                        0);
	}

	std::vector<GLenum> drawBuffers;
	if (type & TextureType::Color || type & TextureType::Lighting)
		drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0_EXT);
	if (type & TextureType::Depth)
		drawBuffers.emplace_back(GL_DEPTH_ATTACHMENT);

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

void RenderTexture::bind(const RenderTexture* rt)
{
	if (!rt)
		glFlush();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt != nullptr ? rt->fbo : 0);
}
} // namespace Dunjun
