#ifndef DUNJUN_CORE_FILESYSTEM_HPP
#define DUNJUN_CORE_FILESYSTEM_HPP

#include <Dunjun/Core/String.hpp>

namespace Dunjun
{
namespace FileSystem
{
String getBasePath();
String getPreferencesPath(const String& organization,
                          const String& application);
String getFileDirectory(const String& filepath);
} // namespace FileSystem
} // namespace Dunjun

#endif
