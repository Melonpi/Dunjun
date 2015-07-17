#ifndef DUNJUN_CORE_CLOCK_HPP
#define DUNJUN_CORE_CLOCK_HPP

#include <Dunjun/Core/Time.hpp>

namespace Dunjun
{
// Utility type that measures the elapsed time
struct Clock
{
	Time startTime = Time::now();

	// `getElapsedTime` returns the time elapsed since the last call to
	// `restart()` or its construction
	inline Time getElapsedTime() const { return Time::now() - startTime; }

	// Restart the clock
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
