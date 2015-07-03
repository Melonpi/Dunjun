#ifndef DUNJUN_WINDOW_GLCONTEXTSETTINGS_HPP
#define DUNJUN_WINDOW_GLCONTEXTSETTINGS_HPP

#include <Dunjun/Common.hpp>

namespace Dunjun
{
struct GLContextSettings
{
	u32 depthBits         = 0;
	u32 stencilBits       = 0;
	u32 antialiasingLevel = 0; // TODO(bill): We are using Deferred Shading
	                           // show this could be number of passes
	u32 majorVersion = 2;
	u32 minorVersion = 1;
	// TODO(bill): Anisotropic?

	explicit GLContextSettings(u32 depthBits         = 0,
	                           u32 stencilBits       = 0,
	                           u32 antialiasingLevel = 0,
	                           u32 majorVersion      = 2,
	                           u32 minorVersion      = 1)
	: depthBits{depthBits}
	, stencilBits{stencilBits}
	, antialiasingLevel{antialiasingLevel}
	, majorVersion{majorVersion}
	, minorVersion{minorVersion}
	{
	}
};
} // namespace Dunjun

#endif
