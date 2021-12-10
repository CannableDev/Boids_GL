#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Transform.h"
#include <glm\fwd.hpp>

class RigidBody {
public:
	RigidBody(Transform* t);
	RigidBody() = default;

	void SetPhysics(float friction, float rotFriction);

	void Update();
	void Update(glm::vec3 vel, glm::quat rot);
	void Update(glm::vec3 vel, glm::vec3 rotTo);

	const glm::vec3& GetVel();

private:
//	RigidBody() = delete;
	friend class DynamicObject;
	Transform* transform;

	glm::vec3 velocity;
	glm::quat rotVelocity;

	float friction, rotFriction;
};

#endif
