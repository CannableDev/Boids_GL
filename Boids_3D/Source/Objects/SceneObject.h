#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "../Shaders/Shader.h"
#include "../RenderObjects/Model.h"
#include "../Utility/Transform.h"
#include "../Utility/Collider.h"
#include "../Utility/BoundingBox.h"
#include <glm/glm.hpp>

class GridCell;
class Light;

class SceneObject {
public:
	SceneObject(Transform* t, Shader* s, Model* m, Collider* c);
	~SceneObject();

	virtual void Update() {};
	virtual void Draw(unsigned int reflectTex, unsigned int shadowTex, Light** lights, unsigned int numLights);
	virtual void DrawShadowPass(Shader& s);

	const bool& isStatic();

	void MarkForDeath();
	const bool& isDead();

	Transform* transform;
	
	Collider* collider;
	BoundingBox* boundbox;

	std::vector<GridCell*> cells;
protected:
	SceneObject(Transform* t, Shader* s, Model* m, Collider* c, bool staticObj);
	SceneObject() = delete;

	Shader* shader;
	Model* model;

	const bool staticObj = true;

	bool toDestroy = false;
};

#endif