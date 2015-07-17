#ifndef DUNJUN_SYSTETICKCOUNTER_HPP
#define DUNJUN_CORE_TICKCOUNTER_HPP

#include <Dunjun/Core/Clock.hpp>

namespace Dunjun
{
struct TickCounter
{
	usize tick   = 0;
	f64 tickRate = 0;
	Clock clock  = {};

	bool update(Time period)
	{
		bool reset = false;
		if (clock.getElapsedTime() >= period)
		{
			tickRate = tick * (1.0f / period.asSeconds());
			tick     = 0;
			reset    = true;
			clock.restart();
		}

		tick++;

		return reset;
	}
};

} // namespace Dunjun

#endif
