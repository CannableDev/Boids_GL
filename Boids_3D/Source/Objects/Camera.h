#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#define CAM_SPEED 33.0f
#define CAM_FRICTION 3.0f
#define CAM_SENSITIVITY 2.6f
#define ROTATION_SPEED 0.5f
#define ROTATION_FRICTION 8.0f
#define BARREL_SPEED 3.f

class Camera {
public:
	static float* GetViewMatrix();
	static void InitProjection();
	static const glm::mat4& GetProjection();
	static const glm::mat4& GetView();

	static void UpdateMouse(float x, float y);
	static void UpdateMovement(float f, float r, float rot);
	static glm::vec3 GetPosition();
	static void SetPosition(glm::vec3 p);
	static glm::vec3 GetForward();
	static void Reset();

	static void Update();

private:
	static glm::mat4 projectionMat;
	static glm::mat4 viewMat;

	static glm::vec3 position;
	static glm::vec3 velocity;
	static glm::quat rotation;
	static glm::quat rotVelocity;

	static float xmouse, ymouse, fwd, right, rotate;

};

#endif