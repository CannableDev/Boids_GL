#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "../Objects/SceneObject.h"
#include <vector>

using glm::vec3;

class GridCell {
public:
	GridCell(vec3 pos, vec3 min, vec3 max, int x, int y);
	~GridCell();

	void Add(SceneObject* o);
	void Remove(SceneObject* o);

	void Draw();

	static bool debugDraw;

	Transform transform;
	BoundingBox* bb;

	std::vector<SceneObject*> objs;

	int x, y;

private:


};

#endif
