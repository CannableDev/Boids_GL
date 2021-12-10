#ifndef UNIGRID_H
#define UNIGRID_H

#include "GridCell.h"

class UniGrid {
public:
	UniGrid(vec3 topleftstart, float size, int dimenison);
	~UniGrid();

	void AddObj(SceneObject* o);
	void RemoveObj(SceneObject* o);

	void TestCollisions(SceneObject* o);

	void Draw();

	static bool debugDraw;
private:
	std::vector<std::vector<GridCell>> grid;
	vec3 startpos;
	float size;
	int dimension;
};

#endif