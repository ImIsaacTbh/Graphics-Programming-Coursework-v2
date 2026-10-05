#include "Core/FrameTimer.h"
#include <iostream>

FrameTimer::FrameTimer()
	: _prevFrameEnd(Clock::now()), _secondStart(Clock::now())
{
}

void FrameTimer::Tick()
{
	Clock::time_point now = Clock::now();

	_deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - _prevFrameEnd).count() / 1000.0;
	_prevFrameEnd = now;
	_framesThisSecond++;

	if (now - _secondStart > std::chrono::seconds(1))
	{
		std::cout << _framesThisSecond << std::endl;
		_framerate = (float)_framesThisSecond;
		_framesThisSecond = 0;
		_secondStart = now;
	}
}
