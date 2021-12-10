#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <sdl\SDL.h>

#define TIME_MULT_MIN 0.05f
#define TIME_MULT_MAX 1.0f

class AppTime {
public:
	AppTime();

	static const float GetElapsed();
	static const float GetDelta();

	static void UpdateTime();
	
	static void SetTimeMultiplier(float mult);
	static float GetTimeMultiplier();

private:
	static float last_t;
	static float time;
	static float delta;
	static float timeMult;
};
#endif