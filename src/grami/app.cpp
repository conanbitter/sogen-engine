#include "glad/gl.h"
#include "app.hpp"
#include <iostream>

App& App::getInstance() {
    static App theInstance;
    return theInstance;
}

void App::initSDL(const std::string& title) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth,
                              windowHeight,
                              SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);
    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    std::cout << "OpenGL version " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    keyboardState = SDL_GetKeyboardState(nullptr);
}

void App::init(const std::string title, int width, int height, int scale) {
    if (isInitComplete) return;

    frameWidth = width;
    frameHeight = height;
    windowWidth = frameWidth * scale;
    windowHeight = frameHeight * scale;

    initSDL(title);
    renderer.init(width, height);
    glViewport(0, 0, windowWidth, windowHeight);

    isInitComplete = true;
}

App::~App() {
    if (context) {
        SDL_GL_DeleteContext(context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void App::run() {
    SDL_Event event;
    isRunning = true;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
            }
        }

        currentScene->onUpdate(1.0f, *this, renderer);

        glClear(GL_COLOR_BUFFER_BIT);
        currentScene->onRender(*this, renderer);
        renderer.update();
        renderer.present();

        SDL_GL_SwapWindow(window);
        SDL_Delay(5);
    }
    currentScene->onUnload(*this, renderer);
}

void App::requestExit() {
    isRunning = false;
}

void App::setScene(Scene* newScene) {
    if (!isInitComplete) return;
    currentScene->onUnload(*this, renderer);
    if (newScene != nullptr) {
        currentScene = newScene;
    } else {
        currentScene = &dummyScene;
    }
    currentScene->onLoad(*this, renderer);
}