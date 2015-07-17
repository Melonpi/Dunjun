#ifndef DUNJUN_WINDOW_VIDEOMODE_HPP
#define DUNJUN_WINDOW_VIDEOMODE_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/Core/ContainerTypes.hpp>

namespace Dunjun
{
struct VideoMode
{
	u32 width        = 0;
	u32 height       = 0;
	u32 bitsPerPixel = 24;

	VideoMode() = default;
	VideoMode(u32 width, u32 height, u32 bitsPerPixel = 24);

	GLOBAL VideoMode getDesktopMode();
	GLOBAL const Array<VideoMode>& getFullscreenModes();

	bool isValid() const;
};

bool operator==(const VideoMode& left, const VideoMode& right);
bool operator!=(const VideoMode& left, const VideoMode& right);
bool operator<(const VideoMode& left, const VideoMode& right);
bool operator>(const VideoMode& left, const VideoMode& right);
bool operator<=(const VideoMode& left, const VideoMode& right);
bool operator>=(const VideoMode& left, const VideoMode& right);
} // namespace Dunjun

#endif
