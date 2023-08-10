#pragma once

#include "glad/gl.h"
#include <vector>
#include <array>
#include "textures.hpp"

class TexturePack;
class Texture;

class Renderer : public Texture {
   public:
    void setColor(uint8_t index, Color color);
    void setPalette(const Palette& colors, uint8_t startIndex = 0, int count = -1);
    void setPalette(const TexturePack& tpak);

    friend class App;

   private:
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint frame_texture;
    GLuint pal_texture;
    std::array<Color, 256> palette;

    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();
    void init(int width, int height);
    void update();
    void update_palette();
    void present();
};
