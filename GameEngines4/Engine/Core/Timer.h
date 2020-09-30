#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>
class Timer
{
public:
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator = (const Timer&) = delete;
	Timer& operator = (Timer&&) = delete;
	//these 4 line make everybody else cannot move or change the timer class
	Timer();

	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;

	unsigned int GetSleepTime(unsigned int fps_) const; 
	//get total time
	float GetCurrentTicks();

private:
	unsigned int prevTicks, currentTicks;
};
#endif // !TIMER


