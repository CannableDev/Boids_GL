#include "SceneWorld.h"

SceneWorld::SceneWorld() : staticGrid({-25, 0, -25}, 5, 10)
{
}

SceneWorld::SceneWorld(std::vector<SceneObject*> physicsObjs, std::vector<SceneObject*> drawnObjs)
	: staticGrid({ -50, 0, -50 }, 5, 10)
{
	for (auto& o : physicsObjs) {
		AddPhysicsObject(o);
	}

	for (auto& o : drawnObjs) {
		AddDrawnObject(o);
	}

	AddNewObjects();
}

SceneWorld::~SceneWorld()
{
}

void SceneWorld::PhysicsUpdate()
{
	// for (auto& o : dynamics) o->Update();
	// for (auto& o : statics) o->Update();
	for (auto& o : dynamics) {

	}
}

void SceneWorld::AddDrawnObject(SceneObject* s)
{
	newDrawn.push_back(s);
}

void SceneWorld::AddPhysicsObject(SceneObject* s)
{
	if (s->isStatic()) {
		statics.push_back(s);
		staticGrid.AddObj(s);
	}
	else {
		newDynamics.push_back(s);
	}
}

std::vector<SceneObject*>& SceneWorld::GetDrawn()
{
	return drawn;
}

std::vector<SceneObject*>& SceneWorld::GetDynamics()
{
	return dynamics;
}

std::vector<SceneObject*>& SceneWorld::GetStatics()
{
	return statics;
}

void SceneWorld::Cleanup()
{
	int counter = 0;

	for (int i = 0; i < dynamics.size() - counter; i++)
		if (dynamics[i]->isDead()) {
			std::cout << "Killing dynamic object " << dynamics[i] << std::endl;
			std::swap(dynamics[i], dynamics[dynamics.size() - 1 - counter]);
			counter++;
		}

	dynamics.erase(dynamics.end() - counter, dynamics.end());

	counter = 0;
	for (int i = 0; i < drawn.size() - counter; i++)
		if (drawn[i]->isDead()) {
			std::cout << "Killing drawn object " << drawn[i] << std::endl;
			std::swap(drawn[i], drawn[drawn.size() - 1 - counter]);
			counter++;
		}

	drawn.erase(drawn.end() - counter, drawn.end());
	AddNewObjects();
}

void SceneWorld::DrawUniformGrid()
{
	if (UniGrid::debugDraw) staticGrid.Draw();
}

void SceneWorld::AddNewObjects()
{
	for (auto& o : newDrawn) {
		drawn.push_back(o);
	}

	newDrawn.clear();

	for (auto& o : newDynamics) {
		dynamics.push_back(o);
	}

	newDynamics.clear();
}
