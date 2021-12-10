#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include "../Objects/Camera.h"
#include "../Objects/GUI.h"
#include "FPSCounter.h"
#include "../Utility/SceneWorld.h"
#include "../Objects/Swarm.h"
#include "../RenderObjects/Skybox.h"
#include "../RenderObjects/Light.h"
#include "../RenderObjects/Renderer.h"

#define MAX_LIGHTS 10

class Scene {
public:
	Scene(std::string n, int w, int h, GUI* g);
	Scene(int w, int h, GUI* g);
	virtual ~Scene();

	virtual void Init();

	virtual void Draw();
	virtual void DrawGUI();
	virtual void Update();
	
	virtual Light* CreateLight(Light* l);
	virtual bool DeleteLight(Light* l);
	virtual const std::vector<Light*> GetLights();

	static Scene* current;

	bool SetAsCurrent();

protected:
	std::string name;

	Skybox skybox;

	SceneWorld* world;

	Swarm* swarm;

	FPSCounter fps;
	unsigned int width, height;

	static bool DISPLAY_DEBUG;

	GUI* gui;

	Light* lights[MAX_LIGHTS];
	int numLights = 0;

	Renderer renderer;

};

#endif