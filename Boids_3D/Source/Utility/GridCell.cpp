#include "GridCell.h"

bool GridCell::debugDraw = false;

// y axis scale set to 0 so drawing isn't wack
GridCell::GridCell(vec3 pos, vec3 min, vec3 max, int x, int y)
	: transform(pos, {1,0,0,0}, {1,0,1}), bb(nullptr), x(x), y(y)
{
	bb = new BoundingBox(min, max, &transform);
}

GridCell::~GridCell()
{
	if (!bb) delete bb;
}

void GridCell::Add(SceneObject* o)
{
	for (auto& ob : objs)
		if (o == ob) return;
	objs.push_back(o);
	o->cells.push_back(this);
}

void GridCell::Remove(SceneObject* o)
{
	for (int i = 0; i < objs.size(); i++) {
		if (o == objs[i]) {
			std::swap(objs[i], objs[objs.size()-1]);
			objs.erase(objs.end());
		}
	}
}

void GridCell::Draw()
{
	bb->Draw({ objs.size(), 0.2, 1 });
}
