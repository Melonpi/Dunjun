#include <Dunjun/Common.hpp>

#include <SDL/SDL.h>

#include <chrono>
#include <thread>

namespace Dunjun
{
std::string getFileDirectory(const std::string& filepath)
{
	usize found = filepath.find_last_of("/\\");
	return filepath.substr(0, found);
}

bool showSimpleMessageBox(MessageBoxType type,
                          const std::string& title,
                          const std::string& message)
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
	           flag, title.c_str(), message.c_str(), nullptr) == 0;
}

namespace BaseDirectory
{
const std::string Textures = "data/textures/";
const std::string Shaders = "data/shaders/";
} // namespace BaseDirectory
} // namespace Dunjun
