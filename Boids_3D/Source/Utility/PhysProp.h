#ifndef PHYSPROP_H
#define PHYSPROP_H

#include <glm/glm.hpp>

#define GRAVITY glm::vec3(0.0, -10.0, 0.0)

struct PhysProp {
	float bounce;
	float friction;
	float mass;
};

#endif