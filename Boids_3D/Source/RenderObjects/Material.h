#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <assimp/scene.h>

struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float reflectiveness;

	Material() : diffuse(glm::vec3(0, 0, 0)), specular(glm::vec3(0, 0, 0)), shininess(0), reflectiveness(0) {}

	Material(aiColor3D d, aiColor3D s, float sh, float r) : diffuse(d.r, d.g, d.b), specular(s.r, s.g, s.b), shininess(sh), reflectiveness(r) {}
	
	Material(glm::vec3 d, glm::vec3 s, float sh, float r) : diffuse(d), specular(s), shininess(sh), reflectiveness(r) {}

	~Material() = default;
};

#endif 