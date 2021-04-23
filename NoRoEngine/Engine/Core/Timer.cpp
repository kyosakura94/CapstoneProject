#include "Timer.h"

Timer::Timer(): prevTicks(0), currentTicks(0)
{

}

void Timer::Start()
{
	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks() {
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

float Timer::GetDeltaTime() const {
	//convert to second
	//static cast to float before divide

	return static_cast<float>(currentTicks - prevTicks) / 1000.0f;
}

unsigned int Timer::GetSleepTime(unsigned int fps_) const
{
	unsigned int milliPerFrame = 1000 / fps_;
	if (milliPerFrame == 0)
	{
		return 0;
	}

	unsigned int sleepTime = milliPerFrame - SDL_GetTicks();
	if (sleepTime > milliPerFrame)
	{
		return milliPerFrame;
	}

	return sleepTime;
}

float Timer::GetCurrentTicks()
{
	//devide to 1000.f in order to change to seconds
	return static_cast<float> (currentTicks) / 1000.0f;
}

