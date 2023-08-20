#pragma once

#include <vector>
#include <string>
#include <map>
#include <utility>

struct Rect {
    int x;
    int y;
    int w;
    int h;

    Rect(int x, int y, int width, int height) : x{x}, y{y}, w{width}, h{height} {}
};

#pragma pack(push, 1)
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color(uint8_t red, uint8_t green, uint8_t blue) : r{red}, g{green}, b{blue}, a{255} {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r{red}, g{green}, b{blue}, a{alpha} {}
    // TODO operators
    Color() : r{0}, g{0}, b{0}, a{255} {}
};
#pragma pack(pop)

class Texture {
   public:
    Texture(int texWidth, int texHeight) : width{texWidth}, height{texHeight}, data(texWidth * texHeight) {}
    Texture(std::string filename);  // TODO
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void copy(const Texture& other);
    void clear(const Color color);
    void blit(const Texture& tex, int x = 0, int y = 0);                                                                      // TODO
    void blit(const Texture& tex, int x, int y, const Rect& srcRect);                                                         // TODO
    void blit(const Texture& tex, const Rect& dstRect, const Rect& srcRect);                                                  // TODO
    void blitAlpha(const Texture& tex, int x = 0, int y = 0, float alpha, bool isPremultiplied = false);                      // TODO
    void blitAlpha(const Texture& tex, int x, int y, const Rect& srcRect, float alpha, bool isPremultiplied = false);         // TODO
    void blitAlpha(const Texture& tex, const Rect& dstRect, const Rect& srcRect, float alpha, bool isPremultiplied = false);  // TODO
    // pixel access with bounds check
    Color getPixel(int x, int y) const;
    Color getPixel(float x, float y) const;
    void putPixel(int x, int y, const Color color);
    // raw pixel accsess without bounds check
    Color getPixelRaw(int x, int y) const { return data[x + y * width]; }
    void putPixelRaw(int x, int y, const Color color) { data[x + y * width] = color; }
    Color getPixelRaw(float x, float y) const;
    Color& operator[](int index) { return data[index]; }
    Color& operator[](std::pair<int, int> coords) { return data[coords.first + coords.second * width]; }

   protected:
    Texture() : width{0}, height{0}, data(0) {}
    int width;
    int height;
    std::vector<Color> data;
    void wrapCoords(int& x, int& y) const;
};