#include "SceneObject.h"
#include "../RenderObjects/Light.h"
#include "../Objects/Camera.h"
#include "../Utility/GridCell.h"
#include "../RenderObjects/Renderer.h"

SceneObject::SceneObject(Transform* t, Shader* s, Model* m, Collider* c)
	: transform(t), shader(s), model(m), collider(c), boundbox(new BoundingBox(m->min, m->max, transform))
{
	collider->obj = this;
	collider->transform = transform;
	// boundbox.Rebuild();
}

SceneObject::SceneObject(Transform* t, Shader* s, Model* m, Collider* c, bool staticObj)
	: transform(t), shader(s), model(m), collider(c), staticObj(staticObj), 
	boundbox(new BoundingBox(m->min, m->max, transform))
{
	collider->obj = this;
	collider->transform = transform;
	// boundbox.Rebuild();
}

SceneObject::~SceneObject()
{
	if (!model) delete model;
	if (!shader) delete shader;
	if (!collider) delete collider;
	if (!transform) delete transform;
	for (auto& c : cells) {
		c->Remove(this);
	}
}

void SceneObject::Draw(unsigned int reflectTex, unsigned int shadowTex, Light** lights, unsigned int numLights)
{
	shader->use();
	shader->setMat4("projection", Camera::GetProjection());
	shader->setMat4("view", Camera::GetView());
	shader->setMat4("model", transform->ToMatrix());
	shader->setVec3("cameraPos", Camera::GetPosition());
	shader->setMat4("lightSpaceMatrix", Renderer::lightSpaceMatrix);

	for (int i = 0; i < numLights; i++) {
		std::string s = "lights[" + std::to_string(i) + "]";
		shader->setLightProperties(s, lights[i]->properties);
	}

	shader->setInt("lightNumber", numLights);
	model->Draw(*shader, reflectTex, shadowTex);

	if (BoundingBox::debugDraw) boundbox->Draw({ 0,1,0 });
	if (Collider::debugDraw) collider->Draw();
}

void SceneObject::DrawShadowPass(Shader& s)
{
	s.setMat4("model", transform->ToMatrix());
	model->DrawShadowPass();
}

const bool& SceneObject::isStatic()
{
	return staticObj;
}

void SceneObject::MarkForDeath()
{
	toDestroy = true;
}

const bool& SceneObject::isDead()
{
	return toDestroy;
}
