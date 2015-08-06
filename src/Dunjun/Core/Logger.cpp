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
	strcpy(s_buffer, logger.prefix);
	strncat(s_buffer, " ", 1);

	time_t rawTime = {};
	struct tm* timeInfo = nullptr;

	if (logger.flags & LogFlag_Date | logger.flags & LogFlag_Date)
	{
		time(&rawTime);
		timeInfo = localtime(&rawTime);
	}
	if (logger.flags & LogFlag_Date)
	{
		LOCAL_PERSIST char s_dateBuffer[13] = {};
		_snprintf(s_dateBuffer, 13,
		          "%04d-%02d-%02d ",
		          1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday);
		strncat(s_buffer, s_dateBuffer, 13);
	}
	if (logger.flags & LogFlag_Time)
	{
		LOCAL_PERSIST char s_timeBuffer[13] = {};
		_snprintf(s_timeBuffer, 13,
		          "%02d:%02d:%02d ",
		          timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
		strncat(s_buffer, s_timeBuffer, 13);
	}
	strncat(s_buffer, fmt, strlen(fmt));

	va_list v;
	va_start(v, fmt);
	{
		vfprintf(logger.file, s_buffer, v);
	}
	va_end(v);
}
} // namespace Dunjun
