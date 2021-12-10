#include "Application.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include <stdlib.h>  
#include "AppTime.h"

Application::Application(const char* title, bool fullscreen, int width, int height)
    : mainWindowTitle(title),
    fullScreen(fullscreen),
    windowWidth(width),
    windowHeight(height),
    screenWidth(0),
    screenHeight(0),
    window(nullptr),
    glContext(),
    event(),
    quitApp(false),
    gui(nullptr)
{}

int Application::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "APP:ERROR: SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    if (fullScreen == true) {
        window = SDL_CreateWindow(mainWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    }
    else {
        window = SDL_CreateWindow(mainWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    }

    glContext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "APP:ERROR: Failed to initialize the OpenGL context." << std::endl;
        return -3;
    }
    
    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        screenWidth = dm.w;
        screenHeight = dm.h;
        if (fullScreen == true) {
            windowWidth = screenWidth;
            windowHeight = screenHeight;
        }
    }
    else {
        std::cerr << "APP:WARNING: SDL coudn't retrieve current display mode details." << std::endl;
    }

    
    // relative mouse movement
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // vsync settings - use adaptive if possible
    if (SDL_GL_SetSwapInterval(-1) < 0) SDL_GL_SetSwapInterval(1);
    // SDL_GL_SetSwapInterval(0);
    
    ///////////////////////
    // Create Scene Here //
    ///////////////////////

    std::string name = "Boids Testing";
    displayDetails = "Display: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight) + " | Refresh Rate: " + std::to_string(dm.refresh_rate);
    gui = new GUI(name.c_str(), displayDetails.c_str());
    new Scene(name, windowWidth, windowHeight, gui);
    
    BoundingBox::debugDraw = false;
    Collider::debugDraw = false;
    Light::debugDraw = false;
    UniGrid::debugDraw = false;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::cout << "APP:MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;
    return 0;
}

void Application::Run()
{
    while (!quitApp) {
        quitApp = Tick();
    }
}

void Application::CheckInput()
{
    float fwd, right, rotate, x, y;
    fwd = 0;
    right = 0;
    rotate = 0;
    x = 0;
    y = 0;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    //continuous-response keys
    if (keystate[SDL_SCANCODE_W])
    {
        // std::cout << "FOWARD\n";
        fwd++;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        // std::cout << "BACK\n";
        fwd--;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        // std::cout << "LEFT\n";
        right--;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        // std::cout << "RIGHT\n";
        right++;
    }
    if (keystate[SDL_SCANCODE_Q]) 
    {
        rotate--;
    }
    if (keystate[SDL_SCANCODE_E])
    {
        rotate++;
    }

    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        case SDL_QUIT:
            quitApp = true;
            break;

        case SDL_MOUSEMOTION:
            x += event.motion.xrel;
            y += event.motion.yrel;
            break;

        case SDL_KEYDOWN: // for single presses
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quitApp = true;
                break;
            case SDLK_l:
                Light::debugDraw = !Light::debugDraw;
                break;
            case SDLK_b:
                BoundingBox::debugDraw = !BoundingBox::debugDraw;
                break;
            case SDLK_c:
                Collider::debugDraw = !Collider::debugDraw;
                break;
            case SDLK_u:
                UniGrid::debugDraw = !UniGrid::debugDraw;
                break;
            }
        }
    }

    Camera::UpdateMouse(x, y);
    Camera::UpdateMovement(fwd, right, rotate);
}

void Application::Update()
{
    Camera::Update();
    Scene::current->Update();
}

void Application::Draw()
{
    Scene::current->Draw();

    SDL_GL_SwapWindow(window);
}

bool Application::Tick()
{
    AppTime::UpdateTime();

    CheckInput();
    Update();
    Draw();

    return quitApp;
}

void Application::Done()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    delete gui;
    delete Scene::current;
    window = nullptr;
    glContext = nullptr;
}

inline void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
    
}