#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

class Transform {
public:
	Transform();
	Transform(const Transform& t);
	Transform(glm::vec3 p, glm::quat r, glm::vec3 s);

	glm::mat4 ToMatrix();
	glm::mat4 ToMatrix_NoTranslate();
	glm::mat4 ToMatrix_NoRotate();
	glm::mat4 ToMatrix_NoScale();
	glm::mat4 TranslateMat();
	glm::mat4 RotateMat();
	glm::mat4 ScaleMat();


	glm::vec3 position, scale;
	glm::quat rotation;
};

#endif