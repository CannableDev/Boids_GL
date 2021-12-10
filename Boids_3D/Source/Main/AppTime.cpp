#include "AppTime.h"

float AppTime::last_t = 0;
float AppTime::time = 0;
float AppTime::delta = 0;
float AppTime::timeMult = 1;

AppTime::AppTime() {}

const float AppTime::GetElapsed()
{
	return time / 1000 * timeMult;
}

const float AppTime::GetDelta()
{
	return delta / 1000 * timeMult;
}

void AppTime::UpdateTime()
{
	last_t = time;
	time = SDL_GetTicks();

	delta = time - last_t;
}

void AppTime::SetTimeMultiplier(float mult)
{
	timeMult = mult > TIME_MULT_MAX ? TIME_MULT_MAX : (mult < TIME_MULT_MIN ? TIME_MULT_MIN : mult);
}

float AppTime::GetTimeMultiplier()
{
	return timeMult;
}