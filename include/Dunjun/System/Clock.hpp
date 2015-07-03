#ifndef DUNJUN_SYSTEM_CLOCK_HPP
#define DUNJUN_SYSTEM_CLOCK_HPP

#include <Dunjun/System/Time.hpp>

namespace Dunjun
{
struct Clock
{
	Time startTime = Time::now();

	inline Time getElapsedTime() const { return Time::now() - startTime; }

	inline Time restart()
	{
		Time now     = Time::now();
		Time elapsed = now - startTime;
		startTime    = now;

		return elapsed;
	}
};
} // namespace Dunjun

#endif
