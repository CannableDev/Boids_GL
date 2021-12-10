#include "UniGrid.h"
#include <glm/gtx/string_cast.hpp>
#include "Collisions.h"

bool UniGrid::debugDraw = true;

UniGrid::UniGrid(vec3 topleftstart, float size, int dimension) : startpos(topleftstart), size(size), dimension(dimension)
{

	for (int i = 0; i <= dimension; i++) {
		std::vector<GridCell> line;
		for (int j = 0; j <= dimension; j++) {
			vec3 pos = topleftstart + vec3(size * j, 0, size * i);
			vec3 min = { -size * 0.5 , -100, -size * 0.5 };
			vec3 max = { size * 0.5 , 100, size * 0.5 };

			line.emplace_back(pos, min, max, j, i);
		}
		grid.push_back(line);
	}

}

UniGrid::~UniGrid()
{
	grid.clear();
}

void UniGrid::AddObj(SceneObject* o)
{
	vec3 gridmin = startpos + vec3(-0.5,-0.5,-0.5) * size;
	vec3 gridmax = startpos + vec3(0.5, 0.5, 0.5) * size;
	// working out if we take up more than one space
	vec3 min = (o->boundbox->min - gridmin) / size;
	vec3 max = (o->boundbox->max - gridmax) / size;
	
	for (int y = min.z; y <= max.z+1; y++) {
		for (int x = min.x; x <= max.x+1; x++) {
			/*std::cout << "GRID" << glm::to_string(grid[y][x].transform.position) << " BOUNDS " 
				<< glm::to_string(grid[y][x].transform.position + vec3(-0.5, -0.5, -0.5) * size) 
				<< " " << glm::to_string(grid[y][x].transform.position + vec3(0.5, 0.5, 0.5) * size) << endl;
			
			std::cout << "OBJ" << glm::to_string(o->transform.position) << " BOUNDS " 
				<< glm::to_string(o->boundbox.min) << " " << glm::to_string(o->boundbox.max) << endl;*/
			
			if (grid[y][x].bb->TestAgainst(o->boundbox)) grid[y][x].Add(o);
		}
	}
}

void UniGrid::RemoveObj(SceneObject* o)
{
}

void UniGrid::TestCollisions(SceneObject* o)
{
	vec3 gridmin = startpos + vec3(-0.5, -0.5, -0.5) * size;
	vec3 gridmax = startpos + vec3(0.5, 0.5, 0.5) * size;
	// working out if we take up more than one space
	vec3 min = (o->boundbox->min - gridmin) / size;
	vec3 max = (o->boundbox->max - gridmax) / size;

	for (int y = min.z; y <= max.z + 1; y++) {
		for (int x = min.x; x <= max.x + 1; x++) {
			if (grid[y][x].bb->TestAgainst(o->boundbox)) {

				for (auto& g : grid[y][x].objs) {
					if (o != g)
						if (o->boundbox->TestAgainst(g->boundbox))
							Collisions::Test(o->collider, g->collider);
				}

			}
		}
	}
}

void UniGrid::Draw()
{
	for (auto& v : grid) {
		for (auto& c : v) {
			c.Draw();
		}
	}
}

