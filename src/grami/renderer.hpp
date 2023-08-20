#pragma once

#include "glad/gl.h"
#include <vector>
#include <array>
#include "textures.hpp"

class TexturePack;
class Texture;

class Renderer : public Texture {
    friend class App;

   private:
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint frame_texture;

    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();
    void init(int width, int height);
    void update();
    void present();
};
