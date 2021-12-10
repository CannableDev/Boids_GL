#include "Swarm.h"
#include "../Utility/Random.h"
#include "Camera.h"
#include "../RenderObjects/Light.h"
#include <glm/gtx/string_cast.hpp>
#include "../Utility/Random.h"
#include "../Main/AppTime.h"

vec3 Swarm::chasePos = vec3(0, 0, 0);

Swarm::Swarm(int count)
    : boidCount(count), swarmLight(nullptr),
    SceneObject(new Transform({ 0,1000000,0 }, { 1,0,0,0 }, { 0.35,0.35,0.35 }),
        new Shader("./Source/Shaders/boids.vert", "./Source/Shaders/boids.frag"),
        new Model("./Source/Models/boid/boid.obj", true, false),
        new SphereCollider({ 1,1,1 }, 0.1)
        )
{
    boids.reserve(boidCount);
    models.reserve(boidCount);

    swarmChaseTimer = Random::GetFloat(0.5, 6);
    chasePos = vec3(Random::GetFloat(-20, 20), Random::GetFloat(5, 12), Random::GetFloat(-20, 20));

    collider->active = false;

	for (int i = 0; i < boidCount; i++) {
        float degrees = Random::GetFloat(0, 360.0);
        boids.push_back(Boid(new Transform(glm::vec3(Random::GetFloat(-30.0, 30.0), Random::GetFloat(-30.0, 30.0), Random::GetFloat(-30.0, 30.0)),
            glm::angleAxis(glm::radians(degrees), glm::normalize(glm::vec3(Random::GetFloat(0, 1), Random::GetFloat(0, 1), Random::GetFloat(0,1)))),
            transform->scale), i, shader, model));

        models.emplace_back(boids[i].GetModelMat());
	}

    glBindVertexArray(model->meshes[0].VAO);

    glGenBuffers(1, &VBO_Models);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Models);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * boidCount, &models[0], GL_DYNAMIC_DRAW);
    // Setup the attribute array pointerand enable it :
    // max size of allocation for a vert attribute is a vec4
    // because we're using matricies we have to allocate the first attrib and the following 3
    std::size_t vec4Size = sizeof(glm::vec4);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // unbind
    glBindVertexArray(0);
}

Swarm::~Swarm()
{
    if (VBO_Models != 0) glDeleteBuffers(1, &VBO_Models);
    
    boids.clear();
    models.clear();

	delete shader;
}

void Swarm::Update()
{
    models.clear();

    swarmChaseTimer -= AppTime::GetDelta();
    if (swarmChaseTimer <= 0) {
        swarmChaseTimer = Random::GetFloat(0.5, 6);
        chasePos = vec3(Random::GetFloat(-20, 20), Random::GetFloat(5, 12), Random::GetFloat(-20, 20));
    }


    vec3 avgPos(0,0,0);
    vec3 colour();
	for (auto& b : boids) {
        b.Update(boids);
        avgPos += b.transform->position;
        models.push_back(b.GetModelMat());
	}
    avgPos /= (boidCount < 1 ? 1 : boidCount);

    if (swarmLight) {
        swarmLight->properties.position = avgPos;
        swarmLight->properties.diffuse = boidMaxColour;
    }
}

void Swarm::Draw(unsigned int reflectTex, unsigned int shadowTex, Light** lights, unsigned int numLights)
{
    shader->use();
    shader->setMat4("projection", Camera::GetProjection());
    shader->setMat4("view", Camera::GetView());
    shader->setMat4("model", transform->ToMatrix());
    shader->setVec3("cameraPos", Camera::GetPosition());

    for (int i = 0; i < numLights; i++) {
        std::string s = "lights[" + std::to_string(i) + "]";
        shader->setLightProperties(s, lights[i]->properties);
    }

    shader->setInt("lightNumber", numLights);
    
    glBindVertexArray(model->meshes[0].VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Models);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * models.size(), &models[0]);

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    // send material
    shader->setMaterial("material", model->meshes[0].materialProp);
    unsigned int i = 0;
    for (i; i < model->meshes[0].textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = model->meshes[0].textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        //std::cout << (name + number).c_str() << "  " << i << std::endl;
        shader->setInt((name + number).c_str(), i);
        // and finally bind the texture

        glBindTexture(GL_TEXTURE_2D, model->meshes[0].textures[i].id);
    }

    if (reflectTex != 0) {
        shader->setInt("skybox", i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_CUBE_MAP, reflectTex);
        i++;
    }

    if (shadowTex != 0) {
        shader->setInt("shadow", i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, shadowTex);
        i++;
    }

    // draw mesh
    glDrawElementsInstanced(GL_TRIANGLES, model->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, boidCount);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    if (Collider::debugDraw) {
        for (auto& b : boids)
            b.collider->Draw();
    }

    if (BoundingBox::debugDraw) {
        for (auto& b : boids)
            b.boundbox->Draw({ 0,1,0 });
    }
}

void Swarm::DrawShadowPass(Shader& s)
{
    for (int i = 0; i < boidCount; i++) {
        s.setMat4("model", models[i]);
        model->DrawShadowPass();
    }
}

std::vector<Boid>& Swarm::GetBoids()
{
    return boids;
}
