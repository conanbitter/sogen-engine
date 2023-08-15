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

typedef std::vector<Color> Palette;

class Texture {
   public:
    Texture(int texWidth, int texHeight) : width{texWidth}, height{texHeight}, data(texWidth * texHeight) {}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void clear(Color color);
    void blit(const Texture& tex, int x = 0, int y = 0);
    void blit(const Texture& tex, int x, int y, Rect rect);
    // pixel access with bounds check
    uint8_t pixel(int x, int y) const;
    void pixel(int x, int y, uint8_t color);
    uint8_t pixel(float x, float y) const;
    void pixel(float x, float y, uint8_t color);
    // raw pixel accsess without bounds check
    uint8_t upixel(int x, int y) const;
    void upixel(int x, int y, uint8_t color);
    uint8_t upixel(float x, float y) const;
    void upixel(float x, float y, uint8_t color);
    Color& operator[](int index) { return data[index]; }
    Color& operator[](std::pair<int, int> coords) { return data[coords.first + coords.second * width]; }

   protected:
    Texture() : width{0}, height{0}, data(0) {}
    int width;
    int height;
    std::vector<Color> data;
    void wrapCoords(int& x, int& y) const;
};