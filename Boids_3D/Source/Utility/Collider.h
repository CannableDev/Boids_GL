#ifndef COLLIDER_H
#define COLLIDER_H

#include "PhysProp.h"
#include "Transform.h"
#include "../Shaders/Shader.h"

using glm::vec3;

class SceneObject;

enum class ColliderType {
	SPHERE,
	AABB,
	OBB,
	CAPUSLE,
	MESH,
	POINT
};
// TODO: BOUNDING BOX PLS
class Collider {
public:
	Collider(PhysProp p);
	~Collider();
	
	const PhysProp& GetProp();
	const ColliderType type;
	
	virtual void Draw() = 0;
	virtual void Update() = 0;

	const Transform& GetTransform() {
		return *transform;
	};

	bool active = true;

	static bool debugDraw;

protected:
	Collider(PhysProp p, ColliderType t);
	PhysProp properties;

	friend class SceneObject;

	virtual void DrawSetup() {};

	Transform* transform;
	SceneObject* obj;

	unsigned int VAO;
	unsigned int VBO;

	static Shader* shader;

private:
	void Init();
	static bool initialized;
};

class AABBCollider : public Collider {
public: 
	AABBCollider(PhysProp p, vec3 Cmin, vec3 Cmax);

	void Draw();
	
	void Update();

	vec3 min, max;

	vec3 cube[8];
	vec3 toDraw[36];
private:
	void DrawSetup();
	void GenCube();
};

class SphereCollider : public Collider {
public:
	SphereCollider(PhysProp p, float r);

	void Draw();
	void Update();

	float radius;
};

class PointCollider : public Collider {
public:
	PointCollider(PhysProp p);

	void Draw();
	void Update();
};

class OBBCollider : public Collider {
	void Draw();
	void Update();
};

class CapsuleCollider : public Collider {
	void Draw();
	void Update();
};

class MeshCollider : public Collider {
	void Draw();
	void Update();
};
#endif