#pragma once
#include <chrono>

// Tracks frame delta time and a once-per-second framerate counter.
class FrameTimer
{
public:
	FrameTimer();

	// Call once at the end of every frame.
	void Tick();

	// Duration of the previous frame in seconds (millisecond resolution).
	double GetDeltaTime() const { return _deltaTime; }
	float GetFramerate() const { return _framerate; }

private:
	using Clock = std::chrono::steady_clock;

	Clock::time_point _prevFrameEnd;
	Clock::time_point _secondStart;
	double _deltaTime = 0.0;
	float _framerate = 0.0f;
	int _framesThisSecond = 0;
};
