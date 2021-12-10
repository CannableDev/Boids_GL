#ifndef SWARM_H
#define SWARM_H

#include "Boid.h"
#include "../Shaders/Shader.h"

class Swarm : public SceneObject{
public:
	Swarm(int count);
	
	~Swarm();

	void Update();
	void Draw(unsigned int reflectTex, unsigned int shadowTex, Light** lights, unsigned int numLights) override;
	void DrawShadowPass(Shader& s);

	std::vector<Boid>& GetBoids();

	Light* swarmLight;

	static vec3 chasePos;

private:
	unsigned int boidCount;
	unsigned int VBO_Models;

	std::vector<Boid> boids;
	std::vector<glm::mat4> models;

	float swarmChaseTimer;

	const vec3 boidMaxColour = vec3(1.0, 0.1, 0.0);
	const vec3 boidMinColour = vec3(0.0, 0.0, 0.0);

};

#endif