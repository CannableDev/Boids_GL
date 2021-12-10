#include "Random.h"

bool Random::seeded = false;

float Random::GetFloat(float min, float max)
{
	if (!seeded) { 
		srand(time(NULL));
		seeded = true;
	}

	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

int Random::GetInt(int min, int max)
{
	if (!seeded) {
		srand(time(NULL));
		seeded = true;
	}

	return rand() % (max + 1 - min) + min;
}
