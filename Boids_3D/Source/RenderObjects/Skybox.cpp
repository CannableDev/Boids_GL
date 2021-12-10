#include "Skybox.h"
#include <stb/stb_image.h>
#include <glad/glad.h> 
#include "../Objects/Camera.h"

Skybox::~Skybox()
{
    if (shader != nullptr) delete shader;
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteVertexArrays(1, &VBO);
}

void Skybox::InitCubeMap()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    // attribute location | num of vals per attribute | type | normalization | stride/byte length of ALL vals | byte offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindVertexArray(0);

    shader = new Shader("./Source/Shaders/skybox.vert", "./Source/Shaders/skybox.frag");

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    // order of files matches GL_TEXTURE_CUBE_MAP_XXX id order
    int width, height, channels;
    
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    texture.id = id;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::Draw()
{
    glDepthFunc(GL_LEQUAL);
    
    shader->use();
    shader->setMat4("projection", Camera::GetProjection());
    shader->setMat4("view", Camera::GetView());
    
    glBindVertexArray(VAO); 
    glActiveTexture(GL_TEXTURE0);
    shader->setInt("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(0);
    
    glDepthFunc(GL_LESS);
}
