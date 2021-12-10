#include "Boid.h"
#include "Camera.h"
#include <glm/gtx/string_cast.hpp>
#include "Swarm.h"

#define MAX_DISTANCE 8.0f
#define MIN_DISTANCE 3.0f

#define BOID_SPEED_MOD 0.01f

Boid::Boid(Transform* t, unsigned int id, Shader* sh, Model* m)
	: id(id),
	DynamicObject(t, sh, m, new SphereCollider({ 1,1,1 }, 0.3))
{
}

/*
glm::quat safeQuatLookAt(
	glm::vec3 const& lookFrom,
	glm::vec3 const& lookTo,
	glm::vec3 const& up)
{
	glm::vec3  direction = lookFrom - lookTo;
	float      directionLength = glm::length(direction);

	// Check if the direction is valid; Also deals with NaN
	if (!(directionLength > 0.0001))
		return glm::quat(1, 0, 0, 0); // Just return identity

	// Normalize direction
	direction /= directionLength;

	// Is the normal up (nearly) parallel to direction?
	if (glm::abs(glm::dot(direction, up)) > .9999f) {
		// return identity
		return glm::quat(1, 0, 0, 0);
	}
	else {
		return glm::quatLookAt(direction, up);
	}
}
*/
void Boid::Update(std::vector<Boid>& boids)
{
	if (BoundingBox::debugDraw) {
		boundbox->Rebuild();
	}

	glm::vec3 forward(transform->rotation * glm::vec3(0, 1, 0));

	glm::vec3 vcentre(0, 0, 0);
	glm::vec3 vdisplace(0, 0, 0);
	glm::vec3 vpvelocity(0, 0, 0);
	glm::vec3 target(10, 10, 0);
	
	// if (std::isnan(body.GetVel().x) || std::isnan(body.GetVel().y) || std::isnan(body.GetVel().z)) std::cout << "BOID " << id << " has nan velocity\n";
	// else std::cout << "BOID " << id << " velocity " << glm::to_string(body.GetVel()) << std::endl;

	glm::quat rotDir = glm::quatLookAt(
		glm::normalize(
			glm::length(body.GetVel()) <= 0.01f ? glm::vec3(0,0,-1) : body.GetVel()
		),
		transform->rotation * glm::vec3(0,1,0));
	/*if (id == 1) {
		std::cout << "BOID " << id << " at " << glm::to_string(transform->position) << " mem: " << transform << std::endl;
	}*/
	int closeCount = 0;

	for (auto& b : boids) {
		if (b.id != id) {
			// avoiding nearby shit
			float closeDist = glm::distance(transform->position, b.transform->position);

			if (closeDist < MIN_DISTANCE) {
				vdisplace += (transform->position - b.transform->position) * (MIN_DISTANCE - closeDist);
			}

			if (glm::distance(transform->position, b.transform->position) < MAX_DISTANCE) {
				// centre of flock
				vcentre += b.transform->position;
				
				vpvelocity += b.body.GetVel();
				closeCount++;
			}
		}
	}

	/*std::cout << glm::to_string(vcentre) << std::endl;*/
	vcentre /= (closeCount == 0 ? 1 : closeCount);
	/*std::cout << glm::to_string(vcentre) << std::endl;*/
	vcentre -= transform->position;
	/*std::cout << glm::to_string(vcentre) << std::endl;*/

	vpvelocity /= (closeCount < 1 ? 1 : closeCount);
	vpvelocity /= 9;

	vdisplace *= 100;
	// ----------------- centre of boids | displacement from boids | target pos ---------------
	// OLD
	/*if (std::isnan(vcentre.x) || std::isnan(vcentre.y) || std::isnan(vcentre.z)) std::cout << "BOID " << id << " has nan vecentre\n";
	if (std::isnan(vpvelocity.x) || std::isnan(vpvelocity.y) || std::isnan(vpvelocity.z)) std::cout << "BOID " << id << " has nan vpvelocity\n";
	if (std::isnan(vdisplace.x) || std::isnan(vdisplace.y) || std::isnan(vdisplace.z)) std::cout << "BOID " << id << " has nan vdisplace\n";*/

	glm::vec3 desiredFacingPos((vcentre + vpvelocity + vdisplace + (Swarm::chasePos - transform->position)) * BOID_SPEED_MOD);
	body.Update(desiredFacingPos, rotDir);
	// NEW
	//glm::vec3 desiredFacingPos(vcentre + vdisplace + (Camera::GetPosition() - transform.position));
	//body.Update((transform.rotation * glm::vec3(0, 0, -1)) + vpvelocity, desiredFacingPos);
}

glm::mat4 Boid::GetModelMat()
{
	return transform->ToMatrix();
}