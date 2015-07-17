#include <Dunjun/Core/FileSystem.hpp>

#include <Dunjun/Core/StringFunctions.hpp>

#include <SDL/SDL_filesystem.h>

namespace Dunjun
{
namespace FileSystem
{
String getBasePath() { return {SDL_GetBasePath()}; }

String getPreferencesPath(const String& organization, const String& application)
{
	return {SDL_GetPrefPath(cString(organization), cString(application))};
}

String getFileDirectory(const String& filepath)
{
	const ssize i = Strings::lastIndexAny(filepath, "/\\");
	if (i < 0)
		return filepath;
	return substring(filepath, 0, i);
}
} // namespace FileSystem
} // namespace Dunjun
