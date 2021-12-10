#include "Renderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

glm::mat4 Renderer::lightSpaceMatrix = glm::mat4(1.0);

Renderer::Renderer(int w, int h)
    : quadVAO(0), quadVBO(0), horizontalDraw(true), dirlight(nullptr),
    screenwidth(w), screenheight(h), lightProjection(1.0), lightView(1.0), LightPos(1,1,1),
    shadowDepthShader("./Source/Shaders/shadowDepth.vert", "./Source/Shaders/shadowDepth.frag"),
    shaderBlur("./Source/Shaders/blur.vert", "./Source/Shaders/blur.frag"),
    finalShader("./Source/Shaders/final.vert", "./Source/Shaders/final.frag")
{   
    // configure shadow depth map buffer  
    glGenFramebuffers(1, &shadowDepthFBO);
    // create shadow depth texture
    glGenTextures(1, &shadowDepthTex);
    glBindTexture(GL_TEXTURE_2D, shadowDepthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // already linear because of ortho matrix

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach shadow depth texture as shadow depth FBO's buffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, nearShadow, farShadow);

    // colour + blur draw
    glGenFramebuffers(1, &colourFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, colourFBO);
    glGenTextures(2, colourBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colourBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenwidth, screenheight, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colourBuffers[i], 0);
    }

    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenwidth, screenheight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    attachments[0] = GL_COLOR_ATTACHMENT0;
    attachments[1] = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring

    glGenFramebuffers(2, blurFBO);
    glGenTextures(2, blurColourBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
        glBindTexture(GL_TEXTURE_2D, blurColourBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenwidth, screenheight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColourBuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }

}

Renderer::~Renderer()
{
}

void Renderer::BeginShadowPass(Light* dirlight)
{
    LightPos = -dirlight->properties.position * 18.0f;
    //LightPos = dirlight->properties.position;
    // std::cout << "LIGHT POS FOR DEPTH PASS " << glm::to_string(LightPos) << std::endl;
    lightView = glm::lookAt(LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    shadowDepthShader.use();
    shadowDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
}

Shader& Renderer::GetShadowShader()
{
    return shadowDepthShader;
}

void Renderer::EndShadowPass()
{
    glCullFace(GL_BACK);
    UnbindBuffers();
    glViewport(0, 0, screenwidth, screenheight);
    Clear();
}

void Renderer::Clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BindColourBuff()
{
    glBindFramebuffer(GL_FRAMEBUFFER, colourFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

unsigned int Renderer::GetShadowTexture()
{
    return shadowDepthTex;
}

glm::vec3 Renderer::GetDirLightPos()
{
    return LightPos;
}

void Renderer::UnbindBuffers()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::BlurPass()
{
    shaderBlur.use();
    shaderBlur.setInt("image", 0);
    horizontalDraw = true;
    bool first_iteration = true;
    for (unsigned int i = 0; i < blurAmount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontalDraw]);
        shaderBlur.setInt("horizontal", horizontalDraw);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colourBuffers[1] : blurColourBuffers[!horizontalDraw]);  // bind texture of other framebuffer (or scene if first iteration)
        RenderQuad();
        horizontalDraw = !horizontalDraw;
        if (first_iteration)
            first_iteration = false;
    }

    UnbindBuffers();
}

void Renderer::ColourBloomCombine()
{
    Clear();
    finalShader.use();
    finalShader.setInt("scene", 0);
    finalShader.setInt("bloomBlur", 1);
    finalShader.setInt("bloom", bloom);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blurColourBuffers[!horizontalDraw]);
    RenderQuad();
}

void Renderer::RenderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
