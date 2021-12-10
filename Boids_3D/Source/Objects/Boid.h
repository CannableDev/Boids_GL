#ifndef BOID_H
#define BOID_H

#include "../Utility/RigidBody.h"
#include "DynamicObject.h"
#include <vector>

class Boid : public DynamicObject {
public:
	Boid(Transform* t, unsigned int id, Shader* sh, Model* m);

	void Update(std::vector<Boid>& b);
	glm::mat4 GetModelMat();

private:
	unsigned int id;

};
#endif