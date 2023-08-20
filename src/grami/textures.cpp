#include "textures.hpp"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

constexpr Color Color::Black = Color(0, 0, 0);
constexpr Color Color::White = Color(255, 255, 255);

void Texture::wrapCoords(int &x, int &y) const {
    x %= width;
    if (x < 0) x += width;
    y %= height;
    if (y < 0) y += height;
}

Color Texture::getPixel(int x, int y) const {
    wrapCoords(x, y);
    return data[x + y * width];
}

Color Texture::getPixel(float x, float y) const {
    int ix = width * x;
    int iy = height * y;
    wrapCoords(ix, iy);
    return data[ix + iy * width];
}

void Texture::putPixel(int x, int y, const Color color) {
    wrapCoords(x, y);
    data[x + y * width] = color;
}

Color Texture::getPixelRaw(float x, float y) const {
    int ix = width * x;
    int iy = height * y;
    return data[ix + iy * width];
}

void Texture::clear(const Color color) {
    std::fill(data.begin(), data.end(), color);
}

void Texture::blit(const Texture &tex, int x, int y) {
    blit(tex, x, y, Rect(0, 0, tex.width, tex.height));
}

void Texture::blit(const Texture &tex, int x, int y, const Rect &rect) {
    /*    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
        if (rect.w <= 0 || rect.h <= 0) return;

        for (int ty = 0; ty < rect.h; ty++) {
            std::copy(
                tex.data.begin() + rect.x + (rect.y + ty) * tex.width,
                tex.data.begin() + rect.x + (rect.y + ty) * tex.width + rect.w,
                data.begin() + x + (y + ty) * width);
        }*/
}

void Texture::blitAlpha(const Texture &tex, int x, int y, float alpha, bool isPremultiplied) {
    blitAlpha(tex, x, y, Rect(0, 0, tex.width, tex.height), alpha, isPremultiplied);
}

void Texture::blitAlpha(const Texture &tex, int x, int y, const Rect &rect, float alpha, bool isPremultiplied) {
    /*    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
        if (rect.w <= 0 || rect.h <= 0) return;

        uint8_t tranpColor = tex.transparent_color;
        for (int ty = 0; ty < rect.h; ty++) {
            for (int tx = 0; tx < rect.w; tx++) {
                uint8_t color = tex.data[rect.x + tx + (rect.y + ty) * tex.width];
                if (color == tranpColor) continue;
                data[x + tx + (y + ty) * width] = color;
            }
        }*/
}

void Texture::copy(const Texture &other) {
    std::copy(other.data.begin(), other.data.end(), data.begin());
}

Texture::Texture(const std::string filename) {
    int ch;
    unsigned char *imageData = stbi_load(filename.c_str(), &width, &height, &ch, 4);
    data.resize(width * height);
    clear(Color::White);
    std::copy((Color *)imageData, (Color *)imageData + width * height, data.begin());
    stbi_image_free(imageData);
}