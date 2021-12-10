#ifndef SCENE_WORLD_H
#define SCENE_WORLD_H

#include <vector>

#include "UniGrid.h"
#include "../Objects/DynamicObject.h"

// http://www.mcihanozer.com/tips/computer-graphics/collision-detection-related/uniform-grid-based/

class SceneWorld {
public:
	SceneWorld();
	SceneWorld(std::vector<SceneObject*> physicsObjs, std::vector<SceneObject*> drawnObjs);
	~SceneWorld();

	void PhysicsUpdate();

	void AddDrawnObject(SceneObject* s);
	void AddPhysicsObject(SceneObject* s);

	std::vector<SceneObject*>& GetDrawn();
	std::vector<SceneObject*>& GetDynamics();
	std::vector<SceneObject*>& GetStatics();

	void Cleanup();

	void DrawUniformGrid();

private:
	void AddNewObjects();

	std::vector<SceneObject*> newDrawn;
	std::vector<SceneObject*> drawn;

	std::vector<SceneObject*> newDynamics;
	std::vector<SceneObject*> dynamics;

	UniGrid staticGrid;
	std::vector<SceneObject*> statics;
	
};

#endif