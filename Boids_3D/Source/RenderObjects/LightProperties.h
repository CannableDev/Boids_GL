#ifndef LIGHTPROPERTIES_H
#define LIGHTPROPERTIES_H

#include <glm/glm.hpp>

struct LightProp {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;
	glm::vec3 attenuation;
	bool directional;
};

#endif