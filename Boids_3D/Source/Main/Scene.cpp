#include "Scene.h"
#include <iostream>
#include "../RenderObjects/Model.h"
#include "../Utility/BoundingBox.h"
#include <glm/gtx/string_cast.hpp>

bool Scene::DISPLAY_DEBUG = false;
Scene* Scene::current = nullptr;

Scene::Scene(std::string n, int w, int h, GUI* g) : name(n),
width(w), height(h), gui(g), numLights(0), renderer(w, h)
{
    if (!current) current = this;
    
    // directional
    CreateLight(new Light({
        {0.6,0.6,0.6},
        {1.0,1.0,1.0},
        {0.4,-1.0,1.0},
        {0.6,0.05,0.003},
        true
        }));
    //// point
    CreateLight(new Light({ 
        {0.7, 0.7, 0.6}, 
        { 1.0,1.0,1.0 }, 
        { 10.0,6.0,5.0 },
        { 0.5,0.05,0.003 }, 
        false }));
    CreateLight(new Light({
        {0.5,0.5,0.5},
        {1.0,1.0,1.0},
        {-5.0,15.0,5.0},
        {0.35,0.03,0.001},
        false
        })); 
    
    Init();
}

Scene::Scene(int w, int h, GUI* g) : name("Default Scene"), 
width(w), height(h), gui(g), renderer(w, h)
{
    if (!current) current = this;

    Init();
}

Scene::~Scene()
{
    if (swarm != nullptr) delete swarm;
    if (gui != nullptr) gui = nullptr;
}

void Scene::Init()
{
    Camera::SetPosition(glm::vec3(0, 0, 10));
    Camera::InitProjection();

    skybox.InitCubeMap();

    gui->SetName(name);

    // can pass static objects on creation
    world = new SceneWorld();

    SceneObject* sceneFloor = new SceneObject(new Transform({ 0, 0,0 }, { 1,0,0,0 }, { 3.25 ,1, 3.25 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"), 
        new Model("./Source/Models/plane/plane.obj", false, false), 
        new AABBCollider({ 1,1,1 }, { -26,-5,-26 }, { 26,0,26 }));

    SceneObject* wall = new SceneObject(new Transform({ -5, 0, 8 }, { 1,0,0,0 }, { 1 ,1, 1 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall1.obj", false, false),
        new AABBCollider({ 1,1,1 }, { -6, 0, 7 }, { -4, 4, 9 }));
    
    SceneObject* wall2 = new SceneObject(new Transform({ 21, 0, 15 }, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f)), { 1 ,1, 2 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall2.obj", false, false),
        new AABBCollider({ 1,1,1 }, { 19, 0, 14 }, { 23, 4, 16 }));

    SceneObject* wall3 = new SceneObject(new Transform({ -13, 0,-15 }, { 1,0,0,0 }, { 1 ,1, 1 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall2.obj", false, false),
        new AABBCollider({ 1,1,1 }, { -14, 0, -16 }, { -12, 4, -14 }));

    SceneObject* wall4 = new SceneObject(new Transform({ 3, 0, -6 }, glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)), { 0.5 ,0.5, 0.5 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall1.obj", false, false),
        new AABBCollider({ 1,1,1 }, { 2.5, 0, -6.5 }, { 3.5, 2, -5.5 }));

    SceneObject* wall5 = new SceneObject(new Transform({ 15, 0,-17 }, glm::angleAxis(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)), { 3 ,1.5, 1 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall2.obj", false, false),
        new AABBCollider({ 1,1,1 }, { 14, 0, -20 }, { 16, 6, -14 }));
    
    SceneObject* wall6 = new SceneObject(new Transform({ -3, 0, 20 }, glm::angleAxis(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)), { 1 ,1, 1 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall1.obj", false, false),
        new AABBCollider({ 1,1,1 }, { -4, 0, 19 }, { -2, 4, 21 }));

    SceneObject* wall7 = new SceneObject(new Transform({ -22, 0, 16 }, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), { 1 ,1, 1 }),
        new Shader("./Source/Shaders/default.vert", "./Source/Shaders/default.frag"),
        new Model("./Source/Models/walls/wall1.obj", false, false),
        new AABBCollider({ 1,1,1 }, { -23, 0, 15 }, { -21, 4, 17 }));

    world->AddPhysicsObject(sceneFloor);
    world->AddDrawnObject(sceneFloor);
    world->AddPhysicsObject(wall);
    world->AddDrawnObject(wall);
    world->AddPhysicsObject(wall2);
    world->AddDrawnObject(wall2);
    world->AddPhysicsObject(wall3);
    world->AddDrawnObject(wall3);
    world->AddPhysicsObject(wall4);
    world->AddDrawnObject(wall4);
    world->AddPhysicsObject(wall5);
    world->AddDrawnObject(wall5);
    world->AddPhysicsObject(wall6);
    world->AddDrawnObject(wall6);
    world->AddPhysicsObject(wall7);
    world->AddDrawnObject(wall7);

    swarm = new Swarm(100);

    swarm->swarmLight = CreateLight(new Light({
    { 0.8, 0.2, 0.0 },
    { 0.7, 0.3, 0.3 },
    { 0.0, 0.0, 0.0 },
    { 0.45, 0.05, 0.003 },
    false }));
    
    world->AddDrawnObject(swarm);
    for (auto& b : swarm->GetBoids()) {
        world->AddPhysicsObject(&b);
    }
}

void Scene::Draw()
{
    renderer.Clear();
    //renderer.UnbindBuffers();

    renderer.BeginShadowPass(lights[0]);

    for (auto& o : world->GetDrawn()) {
        o->DrawShadowPass(renderer.GetShadowShader());
    }

    renderer.EndShadowPass();

    renderer.BindColourBuff();
    
    // draw world objs
    for (auto& o : world->GetDrawn()) {
        o->Draw(skybox.GetCubeMapID(), renderer.GetShadowTexture(), lights, numLights);
    }
     
    // draw grid if we need to
    world->DrawUniformGrid();
    // draw lights if we need to
    if (Light::debugDraw) {
        for (size_t i = 0; i < numLights; i++) {
            lights[i]->Draw();
        }
    }

    skybox.Draw();
    
    DrawGUI();

    renderer.BlurPass();

    renderer.ColourBloomCombine();

    // window swap performed outside of scene
}

void Scene::Update()
{
    world->PhysicsUpdate();
    swarm->Update();
    world->Cleanup();
}

Light* Scene::CreateLight(Light* l)
{
    if (numLights >= MAX_LIGHTS) {
        std::cerr << "ERROR: YOU TRIED TO MAKE TOO MANY LIGHTS BUDDY" << std::endl;
        delete l;
        return nullptr;
    }
    else {
        lights[numLights] = l;
        numLights++;
    }
    return l;
}

bool Scene::DeleteLight(Light* l)
{
    if (numLights <= 0) {
        std::cerr << "ERROR: YOU TRIED TO DELETE TOO MANY LIGHTS BUDDY" << std::endl;
        return false;
    }
    else {
        for (int i = 0; i < numLights; i++) {
            // find light
            if (lights[i] == l) {
                delete l;
                lights[i] == nullptr;
                // shuffle all lights down
                for (int j = i; j < numLights - 1; j++) {
                    lights[j] = lights[j + 1];
                    lights[j + 1] = nullptr;
                }

                return true;
            } 
        }
    }
    // if didnt find light return
    return false;
}

const std::vector<Light*> Scene::GetLights()
{
    std::vector<Light*> r;
    for (int i = 0; i < numLights; i++) {
        r.push_back(lights[i]);
    }
    return r;
}

bool Scene::SetAsCurrent()
{
    return current = this;
}

void Scene::DrawGUI()
{
    if (gui != nullptr) {
        gui->SetFPS(fps.GetFPS());
        gui->Draw();
    }
}