#ifndef RR_APPLICATION_H
#define RR_APPLICATION_H

#include <sdl/SDL.h>
#include <string>
#include "Scene.h"
#include "../Objects/GUI.h"

inline void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

class Application {
public:
    Application(const char* title, bool fullscreen = false, int width = 1024, int height = 768);

    int Init();
    void Done();
    void Run();

    void CheckInput();
    void Update();
    void Draw();

    bool Tick();

protected:
    std::string mainWindowTitle;
    std::string displayDetails;

    bool quitApp;
    bool fullScreen;
    int windowWidth;
    int windowHeight;
    int screenWidth;
    int screenHeight;

    SDL_Window* window;
    SDL_GLContext glContext;
    SDL_Event event;
    
    GUI* gui;
};

#endif