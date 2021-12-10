#include "Transform.h"

Transform::Transform() : position(glm::vec3(0, 0, 0)), rotation(glm::quat(1, 0, 0, 0)), scale(glm::vec3(1, 1, 1))
{
}

Transform::Transform(const Transform& t) : position(t.position), rotation(t.rotation), scale(t.scale)
{
}

Transform::Transform(glm::vec3 p, glm::quat r, glm::vec3 s) : position(p), rotation(r), scale(s)
{
}

glm::mat4 Transform::ToMatrix()
{	
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = model * glm::toMat4(rotation) ;
	model = glm::scale(model, scale);

	return model;
}

glm::mat4 Transform::ToMatrix_NoTranslate()
{
	glm::mat4 model(1.0f);
	model = model * glm::toMat4(rotation);
	model = glm::scale(model, scale);

	return model;
}

glm::mat4 Transform::ToMatrix_NoRotate()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);

	return model;
}

glm::mat4 Transform::ToMatrix_NoScale()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = model * glm::toMat4(rotation);

	return model;
}

glm::mat4 Transform::TranslateMat()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, position);

	return model;
}

glm::mat4 Transform::RotateMat()
{
	glm::mat4 model(1.0f);
	model = model * glm::toMat4(rotation);

	return model;
}

glm::mat4 Transform::ScaleMat()
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, scale);

	return model;
}
