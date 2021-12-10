#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "AppTime.h"

class FPSCounter {
public:
	FPSCounter();

	float GetFPS();

private:
	double last;
	double lastfps;
	int frames;

};

#endif