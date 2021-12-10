#include "Collisions.h"


void Collisions::Test(Collider* A, Collider* B)
{
	switch (A->type) {
	case ColliderType::SPHERE:
		switch (B->type) {
		case ColliderType::SPHERE:
			SphereSphere((SphereCollider*)A, (SphereCollider*)B);
			break;
		case ColliderType::POINT:
			SpherePoint((SphereCollider*)A, (PointCollider*)B);
			break;
		case ColliderType::AABB:
			SphereAABB((SphereCollider*)A, (AABBCollider*)B);
			break;
		}
		break;
	case ColliderType::POINT:
		switch (B->type) {
		case ColliderType::SPHERE:
			SpherePoint((SphereCollider*)B, (PointCollider*)A);
			break;
		case ColliderType::POINT:

			break;
		}
		break;
	case ColliderType::AABB:
		switch (B->type) {
		case ColliderType::SPHERE:
			SphereAABB((SphereCollider*)B, (AABBCollider*)A);
			break;
		case ColliderType::AABB:
			AABBAABB((AABBCollider*)A, (AABBCollider*)B);
			break;
		}
		break;
	}
}

void Collisions::SphereSphere(SphereCollider* A, SphereCollider* B)
{
	if (!A->active || !B->active) return;
	Contact c;
	
	/*
	std::cout << "SPHERE SPHERE " << A << " AND " <<
	c.dist = glm::distance(A->position, B->position); B << std::endl;
	
	c.overlap = c.dist - A->radius - B->radius;
	c.Anorm = glm::normalize(B->position - A->position);
	c.Bnorm = glm::normalize(A->position - B->position);
	c.tangent = vec3(-c.Anorm.y, c.Anorm.x, c.Anorm.z);
	*/
	
	// Moving asteroids for no multiple collisions
	//vec3 normUnit = (other->transform->position - transform->position).Normalize();

	//A->transform.position = transform->position + (normUnit * 0.5 * overlap);
	//other->transform->position = other->transform->position - (normUnit * 0.5 * overlap);

	//// deflection

	//vec3 unitTang(-normUnit.y, normUnit.x, normUnit.z);

	//double vel1norm = body->velocity.DotProd(normUnit);
	//double vel1tang = body->velocity.DotProd(unitTang);

	//double vel2norm = other->body->velocity.DotProd(normUnit);
	//double vel2tang = other->body->velocity.DotProd(unitTang);

	//double vel1normP = (vel1norm * (A->radius - B->radius) + 2 * othersize * vel2norm) / (collider->size + othersize);
	//double vel2normP = (vel2norm * (B->radius - A->radius) + 2 * collider->size * vel1norm) / (collider->size + othersize);

	//body->velocity = normUnit * vel1normP + unitTang * vel1tang;
	//other->body->velocity = normUnit * vel2normP + unitTang * vel2tang;
}

void Collisions::SpherePoint(SphereCollider* A, PointCollider* B)
{
	if (!A->active || !B->active) return;
	std::cout << "SPHERE POINT " << A << " AND " << B << std::endl;
}

void Collisions::SphereAABB(SphereCollider* A, AABBCollider* B)
{

}

void Collisions::AABBAABB(AABBCollider* A, AABBCollider* B)
{

}

void Collisions::ResolveStaticCollision(SceneObject* S, DynamicObject* D, Contact C)
{
}

void Collisions::ResolveDynamicCollision(DynamicObject* A, DynamicObject* B, Contact C)
{
}