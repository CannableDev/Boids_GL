#ifndef DYNAMIC_OBJECT_H
#define DYNAMIC_OBJECT_H

#include "SceneObject.h"
#include "../Utility/RigidBody.h"

class DynamicObject : public SceneObject {
public:
	DynamicObject(Transform* t, Shader* s, Model* m, Collider* c);
	~DynamicObject();

	RigidBody body;

private:


};

#endif