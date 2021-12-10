#include "BoundingBox.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Objects/Camera.h"
#include <glm/gtx/component_wise.hpp>
#include <limits>

Shader* BoundingBox::shader = nullptr;
bool BoundingBox::initialized = false;
bool BoundingBox::debugDraw = false;

BoundingBox::BoundingBox(vec3 min, vec3 max, Transform* t) 
	: min(min), max(max), transform(t)
{
	cube[0] = min;
	cube[1] = { min.x, min.y, max.z };
	cube[2] = { min.x, max.y, min.z };
	cube[3] = { max.x, min.y, min.z };
	cube[4] = { min.x, max.y, max.z };
	cube[5] = { max.x, min.y, max.z };
	cube[6] = { max.x, max.y, min.z };
	cube[7] = max;

	Init();
}

BoundingBox::~BoundingBox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void BoundingBox::Draw(vec3 colour)
{
	shader->use();
	shader->setMat4("projection", Camera::GetProjection());
	shader->setMat4("view", Camera::GetView());
	shader->setMat4("model", glm::mat4(1.0));
	shader->setVec3("colour", colour);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool BoundingBox::TestAgainst(BoundingBox* b)
{
	return(max.x > b->min.x &&
		min.x < b->max.x &&
		max.y > b->min.y &&
		min.y < b->max.y&&
		max.z > b->min.z &&
		min.z < b->max.z);
}

void BoundingBox::DestroyRenderObjects()
{
	if (initialized) {
		delete shader;
		initialized = false;
	}
}

void BoundingBox::Rebuild()
{
	GenerateCube();
	SendToGPU();
}

void BoundingBox::Init()
{
	if (!initialized) {
		shader = new Shader("./Source/Shaders/debug.vert", "./Source/Shaders/debug.frag");
		initialized = true;
	}

	GenerateCube();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(toDraw), &toDraw[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void BoundingBox::GenerateCube()
{
	if (transform) {
		min = glm::vec3(1, 1, 1) * std::numeric_limits<float>::max();
		max = glm::vec3(-1, -1, -1) * std::numeric_limits<float>::max();

		for (int i = 0; i < 8; i++) {
			glm::vec3 transformed = transform->ToMatrix() * glm::vec4(cube[i], 1);
			if (transformed.x < min.x) min.x = transformed.x;
			if (transformed.y < min.y) min.y = transformed.y;
			if (transformed.z < min.z) min.z = transformed.z;
			if (transformed.x > max.x) max.x = transformed.x;
			if (transformed.y > max.y) max.y = transformed.y;
			if (transformed.z > max.z) max.z = transformed.z;

		}
	}
	//std::cout << "BB min " << min.x << " " << min.y << " " << min.z << " " << "max " << max.x << " " << max.y << " " << max.z << "\n";
	toDraw[0] = { min.x, min.y, min.z };
	toDraw[1] = { max.x, max.y, min.z };
	toDraw[2] = { max.x, min.y, min.z };
	toDraw[3] = { min.x, min.y, min.z };
	toDraw[4] = { min.x, max.y, min.z };
	toDraw[5] = { max.x, max.y, min.z };

	toDraw[6] = { min.x, min.y, max.z };
	toDraw[7] = { max.x, min.y, max.z };
	toDraw[8] = { max.x, max.y, max.z };
	toDraw[9] = { max.x, max.y, max.z };
	toDraw[10] = { min.x, max.y, max.z };
	toDraw[11] = { min.x, min.y, max.z };

	toDraw[12] = { min.x, max.y, max.z };
	toDraw[13] = { min.x, max.y, min.z };
	toDraw[14] = { min.x, min.y, min.z };
	toDraw[15] = { min.x, min.y, min.z };
	toDraw[16] = { min.x, min.y, max.z };
	toDraw[17] = { min.x, max.y, max.z };

	toDraw[18] = { max.x, max.y, max.z };
	toDraw[19] = { max.x, min.y, min.z };
	toDraw[20] = { max.x, max.y, min.z };
	toDraw[21] = { max.x, max.y, max.z };
	toDraw[22] = { max.x, min.y, max.z };
	toDraw[23] = { max.x, min.y, min.z };

	toDraw[24] = { min.x, min.y, min.z };
	toDraw[25] = { max.x, min.y, min.z };
	toDraw[26] = { max.x, min.y, max.z };
	toDraw[27] = { max.x, min.y, max.z };
	toDraw[28] = { min.x, min.y, max.z };
	toDraw[29] = { min.x, min.y, min.z };

	toDraw[30] = { min.x, max.y, min.z };
	toDraw[31] = { max.x, max.y, max.z };
	toDraw[32] = { max.x, max.y, min.z };
	toDraw[33] = { min.x, max.y, min.z };
	toDraw[34] = { min.x, max.y, max.z };
	toDraw[35] = { max.x, max.y, max.z };

}

void BoundingBox::SendToGPU()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(toDraw), &toDraw[0]);
}
