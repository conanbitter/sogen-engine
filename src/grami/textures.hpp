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

    Color(uint8_t red, uint8_t green, uint8_t blue) : r{red}, g{green}, b{blue} {}
    Color() : r{0}, g{0}, b{0} {}
};
#pragma pack(pop)

typedef std::vector<Color> Palette;

class Texture {
   public:
    Texture(int texWidth, int texHeight) : width{texWidth}, height{texHeight}, data(texWidth * texHeight), transparent_color{-1} {}
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTransparent() const { return transparent_color; }
    void clear(uint8_t color);
    void blit(const Texture& tex, int x = 0, int y = 0);
    void blit(const Texture& tex, int x, int y, Rect rect);
    void blitTransp(const Texture& tex, int x = 0, int y = 0);
    void blitTransp(const Texture& tex, int x, int y, Rect rect);
    // raw copying
    void copy(const std::vector<uint8_t>& other, int offset = 0);
    void copy(const Texture& other);
    // pixel access with bounds check
    uint8_t pixel(int x, int y) const;
    void pixel(int x, int y, uint8_t color);
    // raw pixel accsess without bounds check
    uint8_t& operator[](int index) { return data[index]; }
    uint8_t& operator[](std::pair<int, int> coords) { return data[coords.first + coords.second * width]; }

   protected:
    Texture() : width{0}, height{0}, data(0), transparent_color{-1} {}
    int width;
    int height;
    int transparent_color;
    std::vector<uint8_t> data;
    void wrapCoords(int& x, int& y) const;

    friend class TexturePack;
};

class TexturePack {
   public:
    void load(const std::string filename);
    int count() const { return textures.size(); }
    Texture& getTexture(const std::string name);
    Texture& getTexture(int id);
    int getTextureId(const std::string name);
    const Palette& getPalette() const { return palette; }
    int getPaletteOffset() const { return paletteOffset; }

   private:
    std::vector<Texture> textures;
    std::map<std::string, int> names;
    Palette palette;
    int paletteOffset;

    friend class Renderer;
};