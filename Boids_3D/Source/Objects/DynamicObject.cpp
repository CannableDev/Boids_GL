#include "DynamicObject.h"

DynamicObject::DynamicObject(Transform* t, Shader* s, Model* m, Collider* c)
	: SceneObject(t, s, m, c, false), body(transform)
{
}

DynamicObject::~DynamicObject()
{

}
