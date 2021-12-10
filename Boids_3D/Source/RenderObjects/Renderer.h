#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include "../Shaders/Shader.h"
#include "../RenderObjects/Light.h"

class Renderer {
public:
	Renderer(int w, int h);
	~Renderer();
	
	void Clear(); 
	
	// in order
	// shadows
	void BeginShadowPass(Light* dirlight);
	Shader& GetShadowShader();
	void EndShadowPass();
	// draw + bloom
	void BindColourBuff(); 
	unsigned int GetShadowTexture();
	glm::vec3 GetDirLightPos();
	// blur pass
	void UnbindBuffers();
	void BlurPass();
	// final output
	void ColourBloomCombine();

	// used a lot
	void RenderQuad();

	static glm::mat4 lightSpaceMatrix;

private:
	unsigned int shadowDepthFBO;
	unsigned int shadowDepthTex;

	Shader shadowDepthShader;

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::vec3 LightPos;
	Light* dirlight;
	const float nearShadow = 1.0f; 
	const float farShadow = 70.0f;
	const float borderColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	const unsigned int shadowWidth = 4096;
	const unsigned int shadowHeight = 4096;

	unsigned int colourFBO;
	unsigned int depthRBO;
	unsigned int colourBuffers[2];
	unsigned int attachments[2];

	unsigned int blurFBO[2];
	unsigned int blurColourBuffers[2];
	bool horizontalDraw;
	const bool bloom = true;
	const unsigned int blurAmount = 20;

	unsigned int quadVAO, quadVBO;

	Shader shaderBlur;
	Shader finalShader;

	int screenwidth, screenheight;
};


#endif