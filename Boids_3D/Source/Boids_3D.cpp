#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <sdl/SDL.h>
#define STB_IMAGE_IMPLEMENTATION

// #include <reactphysics3d/reactphysics3d.h>

#include "Main/Application.h"
#include "Objects/UnitSphere.h"
const char* APPLICATION_TITLE = "Boids 3D Demo";

int main(int argc, char* args[])
{

    Application* app = new Application(APPLICATION_TITLE, false, 1920, 1080);
    
    if (int err = app->Init() != 0) {
        std::cout << "APPLICATION:ERROR: Failed to initialise application." << std::endl;
        return -1;
    };

    app->Run();
    app->Done();

    delete app;

    return EXIT_SUCCESS;
}