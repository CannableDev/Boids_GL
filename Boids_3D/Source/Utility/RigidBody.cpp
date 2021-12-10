#include "RigidBody.h"
#include <iostream>
#include "../Main/AppTime.h"


const glm::vec3 zero(0, 0, 0);
const glm::quat identity(1, 0, 0, 0);

#define TOP_SPEED 30.0f
#define TURN_SPEED 1.0f

RigidBody::RigidBody(Transform* t) : transform(t), velocity(), rotVelocity(), friction(1), rotFriction(1)
{
}

void RigidBody::SetPhysics(float f, float rf)
{
	friction = f;
	rotFriction = rf;
}

void RigidBody::Update()
{
	if (!transform) return;
	glm::vec3 vel = velocity * AppTime::GetDelta();

	transform->position = transform->position + vel;
	// mix = lerp for non-quaternions

	velocity = glm::mix(velocity, zero, AppTime::GetDelta() * friction);
	velocity = glm::length(velocity) > TOP_SPEED ? glm::normalize(velocity) * TOP_SPEED : velocity;

	transform->rotation = glm::normalize(glm::slerp(transform->rotation, rotVelocity, 1.0f));
	
	rotVelocity = glm::slerp(rotVelocity, identity, AppTime::GetDelta() * rotFriction);
}

void RigidBody::Update(glm::vec3 vel, glm::quat rot)
{
	velocity += vel;
	rotVelocity = rot;

	Update();
}

void RigidBody::Update(glm::vec3 vel, glm::vec3 rotTo)
{
	velocity += vel;
	
	glm::quat rotDir = glm::quatLookAt(
		glm::normalize(rotTo), 
		transform->rotation * glm::vec3(0, 1, 0));
	// Quaternion relative = Quaternion.Inverse(a) * b;
	rotVelocity = glm::normalize(transform->rotation * glm::inverse(rotDir));

	Update();
}

const glm::vec3& RigidBody::GetVel()
{
	return velocity;
}