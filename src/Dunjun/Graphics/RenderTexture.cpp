#include <Dunjun/Graphics/RenderTexture.hpp>

namespace Dunjun
{
void destroyRenderTexture(RenderTexture& rt)
{
	if (rt.fbo)
		glDeleteFramebuffersEXT(1, &rt.fbo);
	destroyTexture(rt.colorTexture);
	destroyTexture(rt.depthTexture);
}

bool createRenderTexture(RenderTexture& rt,
                         u32 width,
                         u32 height,
                         RenderTexture::TextureType type,
                         TextureFilter minMagFilter,
                         TextureWrapMode wrapMode)
{
	if (width == rt.width && height == rt.height && type == rt.type)
		return true;

	rt.type   = type;
	rt.width  = width;
	rt.height = height;

	glGenFramebuffersEXT(1, &rt.fbo);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt.fbo);
	defer(glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0));

	defer(glBindTexture(GL_TEXTURE_2D, 0));

	if (type & RenderTexture::Color)
	{
		if (!rt.colorTexture.handle)
			glGenTextures(1, &rt.colorTexture.handle);
		glBindTexture(GL_TEXTURE_2D, (u32)rt.colorTexture.handle);
		if (type & RenderTexture::Lighting)
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
		rt.colorTexture.width  = width;
		rt.colorTexture.height = height;

		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

		glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
		                        GL_COLOR_ATTACHMENT0_EXT,
		                        rt.colorTexture.handle,
		                        0);
	}

	if (type & RenderTexture::Depth)
	{
		if (!rt.depthTexture.handle)
			glGenTextures(1, &rt.depthTexture.handle);
		glBindTexture(GL_TEXTURE_2D, (u32)rt.depthTexture.handle);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             GL_DEPTH_COMPONENT24,
		             (s32)width,
		             (s32)height,
		             0,
		             GL_DEPTH_COMPONENT,
		             GL_FLOAT,
		             0);
		rt.depthTexture.width  = width;
		rt.depthTexture.height = height;

		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)minMagFilter);
		glTexParameteri(
		    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode);

		glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT,
		                        GL_DEPTH_ATTACHMENT_EXT,
		                        rt.depthTexture.handle,
		                        0);
	}

	u32 drawBuffersLength = 0;
	GLenum drawBuffers[2];

	if (type & RenderTexture::Color || type & RenderTexture::Lighting)
		drawBuffers[drawBuffersLength++] = GL_COLOR_ATTACHMENT0_EXT;
	if (type & RenderTexture::Depth)
		drawBuffers[drawBuffersLength++] = GL_DEPTH_ATTACHMENT;

	glDrawBuffers(drawBuffersLength, &drawBuffers[0]);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) !=
	    GL_FRAMEBUFFER_COMPLETE_EXT)
		return false;

	return true;
}

void bindRenderTexture(const RenderTexture* rt)
{
	if (!rt)
	{
		glFlush();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	else
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt->fbo);
	}
}
} // namespace Dunjun
