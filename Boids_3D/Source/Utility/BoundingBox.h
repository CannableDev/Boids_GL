#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>
#include <vector>
#include "../Shaders/Shader.h"
#include "Transform.h"

using glm::vec3;

class BoundingBox {
public:
	BoundingBox(vec3 min, vec3 max, Transform* t);
	~BoundingBox();

	void Draw(vec3 colour);

	bool TestAgainst(BoundingBox* b);

	static void DestroyRenderObjects();
	void Rebuild();

	Transform* transform;

	vec3 min;
	vec3 max;

	static bool debugDraw;
private:
	void Init();

	void GenerateCube();
	void SendToGPU();
	
	unsigned int VAO;
	unsigned int VBO;

	vec3 cube[8];
	vec3 toDraw[36];

	static Shader* shader;
	static bool initialized;


};

#endif