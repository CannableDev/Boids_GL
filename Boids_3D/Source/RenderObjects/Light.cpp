#include "Light.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Objects/UnitCube.h"
#include "../Objects/Camera.h"

unsigned int Light::VAO = 0;
unsigned int Light::VBO = 0;
bool Light::debugDraw = false;
bool Light::initialized = false;
Shader* Light::shader = nullptr;

Light::Light() : properties({ 
		{ 0.6, 0.6, 0.6 },
		{ 1.0,1.0,1.0 },
		{ 0.0,0.0,0.0 },
		{ 0.6,0.05,0.003 },
		false })
{
	Init();
}

Light::Light(LightProp p) : properties(p)
{
	Init();
}

Light::~Light()
{
}

void Light::DestroyRenderObjects()
{
	if (initialized) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		
		delete shader;
		VAO = 0;
		VBO = 0;
		initialized = false;
	}
}

void Light::Draw()
{
	if (properties.directional) return;
	shader->use();
	shader->setMat4("projection", Camera::GetProjection());
	shader->setMat4("view", Camera::GetView());
	shader->setMat4("model", glm::translate(glm::mat4(1.0f), properties.position));
	shader->setVec3("colour", LIGHT_DEBUG_COLOUR);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Light::Init()
{
	if (!initialized) {
		shader = new Shader("./Source/Shaders/debug.vert", "./Source/Shaders/debug.frag");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(unitCube), &unitCube[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);

		initialized = true;
	}
}
