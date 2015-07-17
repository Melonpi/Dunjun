#include <Dunjun/Common.hpp>

#include <SDL/SDL.h>

#include <chrono>
#include <thread>

namespace Dunjun
{
bool showSimpleMessageBox(MessageBoxType type,
                          const String& title,
                          const String& message)
{
	u32 flag = 0;

	switch (type)
	{
	case MessageBoxType::Error:
		flag = SDL_MESSAGEBOX_ERROR;
		break;
	case MessageBoxType::Warning:
		flag = SDL_MESSAGEBOX_WARNING;
		break;
	case MessageBoxType::Information:
		flag = SDL_MESSAGEBOX_INFORMATION;
		break;
	}

	return SDL_ShowSimpleMessageBox(
	           flag, cString(title), cString(message), nullptr) == 0;
}
} // namespace Dunjun
