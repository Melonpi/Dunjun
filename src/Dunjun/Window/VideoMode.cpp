#include <Dunjun/Window/VideoMode.hpp>

#include <SDL/SDL_video.h>

#include <algorithm>

namespace Dunjun
{
VideoMode::VideoMode(u32 width, u32 height, u32 bitsPerPixel)
: width{width}
, height{height}
, bitsPerPixel{bitsPerPixel}
{
}

VideoMode VideoMode::getDesktopMode()
{
	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		return {};

	return {(u32)dm.w, (u32)dm.h, SDL_BITSPERPIXEL(dm.format)};
}

const std::vector<VideoMode>& VideoMode::getFullscreenModes()
{
	LOCAL_PERSIST std::vector<VideoMode> modes;

	if (modes.empty())
	{
		printf("Here\n");
		usize displayModeCount;
		SDL_DisplayMode dm;

		displayModeCount = SDL_GetNumDisplayModes(0);

		if (displayModeCount < 1)
		{
			std::cerr << "SDL_GetNumDisplayModes failed: " << SDL_GetError()
			          << std::endl;
			return modes;
		}

		for (usize i{0}; i < displayModeCount; i++)
		{
			if (SDL_GetDisplayMode(0, i, &dm) != 0)
			{
				std::cerr << "SDL_GetNumDisplayModes failed: " << SDL_GetError()
				          << std::endl;
				continue;
			}
			modes.emplace_back(dm.w, dm.h, SDL_BITSPERPIXEL(dm.format));
		}

		std::sort(
		    std::begin(modes), std::end(modes), std::greater<VideoMode>());
	}

	return modes;
}

bool VideoMode::isValid() const
{
	const std::vector<VideoMode>& modes = getFullscreenModes();

	return std::find(std::begin(modes), std::end(modes), *this) !=
	       std::end(modes);
}

bool operator==(const VideoMode& left, const VideoMode& right)
{
	return ((left.width == right.width) && (left.height == right.height) &&
	        (left.bitsPerPixel == right.bitsPerPixel));
}

bool operator!=(const VideoMode& left, const VideoMode& right)
{
	return !(left == right);
}

bool operator<(const VideoMode& left, const VideoMode& right)
{
	if (left.bitsPerPixel == right.bitsPerPixel)
	{
		if (left.width == right.width)
			return left.height < right.height;

		return left.width < right.width;
	}

	return left.bitsPerPixel < right.bitsPerPixel;
}

bool operator>(const VideoMode& left, const VideoMode& right)
{
	return right < left;
}

bool operator<=(const VideoMode& left, const VideoMode& right)
{
	return !(right < left);
}

bool operator>=(const VideoMode& left, const VideoMode& right)
{
	return !(left < right);
}
} // namespace Dunjun
