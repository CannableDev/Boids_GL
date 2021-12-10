#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "../Main/AppTime.h"
#include <iostream>

glm::mat4 Camera::projectionMat = glm::mat4(1.0f);
glm::mat4 Camera::viewMat = glm::mat4(1.0f);
glm::vec3 Camera::position = glm::vec3(0, 0, 0);
glm::vec3 Camera::velocity = glm::vec3(0, 0, 0);
glm::quat Camera::rotation = glm::quat(1, 0, 0, 0);
glm::quat Camera::rotVelocity = glm::quat(1, 0, 0, 0);

float Camera::xmouse = 0;
float Camera::ymouse = 0;
float Camera::right = 0;
float Camera::rotate = 0;
float Camera::fwd = 0;

const glm::vec3 VEC_zero = glm::vec3(0, 0, 0);
const glm::vec3 VEC_forward = glm::vec3(0, 0, -1);
const glm::vec3 VEC_up = glm::vec3(0, 1, 0);
const glm::vec3 VEC_right = glm::vec3(1, 0, 0);
const glm::quat QUAT_identity = glm::quat(1, 0, 0, 0);

float* Camera::GetViewMatrix()
{
	return glm::value_ptr(viewMat);
}

void Camera::InitProjection()
{
	projectionMat = glm::perspective(glm::radians(80.0f), 1920.0f / 1080.0f, 0.01f, 10000.0f);
}

const glm::mat4& Camera::GetProjection()
{
	return projectionMat;
}

const glm::mat4& Camera::GetView()
{
	return viewMat;
}

void Camera::UpdateMouse(float x, float y)
{
	xmouse = -x * CAM_SENSITIVITY;
	ymouse = -y * CAM_SENSITIVITY;
}

void Camera::UpdateMovement(float f, float r, float rot)
{
	fwd = f;
	right = r;
	rotate = rot;
}

glm::vec3 Camera::GetPosition()
{
	return position;
}

void Camera::SetPosition(glm::vec3 p)
{
	position = p;
}

glm::vec3 Camera::GetForward()
{
	return -glm::vec3(rotation * VEC_forward);
}

void Camera::Reset()
{
	viewMat = glm::mat4(1.0f);
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	rotation = glm::quat(1, 0, 0, 0);
	rotVelocity = glm::quat(1, 0, 0, 0);
}

void Camera::Update()
{
	viewMat = glm::mat4(1.0f);

	int rotDir = 0;
	if (rotate > 0) rotDir++;
	if (rotate < 0) rotDir--;

	rotVelocity *= glm::angleAxis(glm::radians(BARREL_SPEED * rotate * AppTime::GetDelta()), VEC_forward);
	rotVelocity *= glm::angleAxis(glm::radians(ROTATION_SPEED * ymouse * AppTime::GetDelta()), VEC_right);
	rotVelocity *= glm::angleAxis(glm::radians(ROTATION_SPEED * xmouse * AppTime::GetDelta()), VEC_up);

	glm::vec3 fwddir(rotation * VEC_forward);
	glm::vec3 rightdir(rotation * VEC_right);

	if (fwd != 0) {

		velocity += (fwddir * fwd) * AppTime::GetDelta();
	} 

	if (right != 0) {
		velocity += (rightdir * right) * AppTime::GetDelta();
	}

	position += (velocity * AppTime::GetDelta() * CAM_SPEED);
	// mix = lerp for non-quaternions
	velocity = glm::mix(velocity, VEC_zero, AppTime::GetDelta() * CAM_FRICTION);

	rotation *= rotVelocity;
	rotation = glm::normalize(rotation);
	rotVelocity = glm::slerp(rotVelocity, QUAT_identity, AppTime::GetDelta() * ROTATION_FRICTION);

	viewMat = glm::toMat4(glm::inverse(rotation)) * glm::translate(viewMat, -position);	
}
