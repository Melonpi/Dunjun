#include <Dunjun/Core/Logger.hpp>

#include <cstdarg>
#include <cstring>
#include <ctime>

namespace Dunjun
{
void logPrintf(const Logger& logger, const char* fmt, ...)
{
	// TODO(bill): Implement Colors and other flags
	LOCAL_PERSIST char s_buffer[1024] = {};
	if (logger.prefix != nullptr)
	{
		strcpy(s_buffer, logger.prefix);
		strncat(s_buffer, " ", 1);
	}
	else
	{
		strcpy(s_buffer, "");
	}

	time_t rawTime      = {};
	struct tm* timeInfo = nullptr;

	// Get Time infor
	if (logger.flags & LogFlag_Date | logger.flags & LogFlag_Date)
	{
		time(&rawTime);
		timeInfo = localtime(&rawTime);
	}
	if (logger.flags & LogFlag_Date)
	{
		LOCAL_PERSIST char s_dateBuffer[13] = {};
		_snprintf(s_dateBuffer,
		          13,
		          "%04d-%02d-%02d ",
		          1900 + timeInfo->tm_year, // tm_year is years since 1900
		          timeInfo->tm_mon + 1,     // tm_mon (0..11)
		          timeInfo->tm_mday);       // tm_mday (1..31)
		strncat(s_buffer, s_dateBuffer, 13);
	}
	if (logger.flags & LogFlag_Time)
	{
		LOCAL_PERSIST char s_timeBuffer[13] = {};
		_snprintf(s_timeBuffer,
		          13,
		          "%02d:%02d:%02d ",
		          timeInfo->tm_hour, // tm_hour (0..23)
		          timeInfo->tm_min,  // tm_min (0..59)
		          timeInfo->tm_sec); // tm_sec (0..60) (leap seconds included)
		strncat(s_buffer, s_timeBuffer, 13);
	}
	strncat(s_buffer, fmt, len(fmt));

	va_list v;
	va_start(v, fmt);
	{
		vfprintf(logger.file, s_buffer, v);
	}
	va_end(v);
}
} // namespace Dunjun
