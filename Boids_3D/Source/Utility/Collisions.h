#ifndef COLLISIONS_UTIL_H
#define COLLISIONS_UTIL_H

#include "Collider.h"
#include "../Objects/DynamicObject.h"

namespace Collisions {

	struct Contact {
		// shit harry wanted
		// SceneObject* objA;
		// SceneObject* objB;
		// points
		vec3 a;
		vec3 b;
		vec3 normal;
		float depth;
	};

	void Test(Collider* A, Collider* B);

	void SphereSphere(SphereCollider* A, SphereCollider* B);

	void SpherePoint(SphereCollider* A, PointCollider* B);

	void SphereAABB(SphereCollider* A, AABBCollider* B);

	void AABBAABB(AABBCollider* A, AABBCollider* B);

	void ResolveStaticCollision(SceneObject* S, DynamicObject* D, Contact C);

	void ResolveDynamicCollision(DynamicObject* A, DynamicObject* B, Contact C);
}

#endif