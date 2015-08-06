#ifndef DUNJUN_CORE_LOGGER_HPP
#define DUNJUN_CORE_LOGGER_HPP

#include <cstdio>

namespace Dunjun
{
enum LogFlag : u32
{
	LogFlag_Date = 1,
	LogFlag_Time = 2,

	LogFlag_Default = LogFlag_Date | LogFlag_Time,
};

struct Logger
{
	FILE* file;
	const char* prefix;
	u32 flags;
};

void logPrintf(const Logger& logger, const char* fmt, ...);

} // namespace Dunjun

#endif
