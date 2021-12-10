#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>
#include "../Shaders/Shader.h"
#include "LightProperties.h"

#define LIGHT_DEBUG_COLOUR {0.8f, 0.3f, 0.6f}

class Light {
public:
	Light();
	Light(LightProp p);
	~Light();

	static void DestroyRenderObjects();

	void Draw();

	LightProp properties;
	static bool debugDraw;

private:
	void Init();

	static unsigned int VAO;
	static unsigned int VBO;
	static Shader* shader;
	static bool initialized; 

};

#endif
