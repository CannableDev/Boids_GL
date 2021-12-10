#ifndef APP_RANDOM_H
#define APP_RANDOM_H

#include <cstdlib>
#include <ctime>

class Random {
public:
	Random() = default;
	static float GetFloat(float min, float max);
	static int GetInt(int min, int max);

private:
	static bool seeded;
};

#endif