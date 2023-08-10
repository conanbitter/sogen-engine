#include "textures.hpp"
#include <fstream>

void Texture::wrapCoords(int &x, int &y) const {
    x %= width;
    if (x < 0) x += width;
    y %= height;
    if (y < 0) y += height;
}

uint8_t Texture::pixel(int x, int y) const {
    wrapCoords(x, y);
    return data[x + y * width];
}

void Texture::pixel(int x, int y, uint8_t color) {
    wrapCoords(x, y);
    data[x + y * width] = color;
}

void Texture::clear(uint8_t color) {
    std::fill(data.begin(), data.end(), color);
}

static void boundsCheck(Rect &rect, int x, int y, int width, int height, int texWidth, int texHeight) {
    if (rect.x < 0) {
        rect.w -= rect.x;
        rect.x = 0;
    }
    if (x < 0) {
        rect.w -= x;
        x = 0;
    }
    if (rect.y < 0) {
        rect.h -= rect.y;
        rect.y = 0;
    }
    if (y < 0) {
        rect.h -= y;
        y = 0;
    }
    if (rect.x + rect.w >= texWidth) {
        rect.w = texWidth - rect.x;
    }
    if (x + rect.w >= width) {
        rect.w = width - x;
    }
    if (rect.y + rect.h >= texHeight) {
        rect.h = texHeight - rect.y;
    }
    if (y + rect.h >= height) {
        rect.h = height - y;
    }
}

void Texture::blit(const Texture &tex, int x, int y) {
    blit(tex, x, y, Rect(0, 0, tex.width, tex.height));
}

void Texture::blit(const Texture &tex, int x, int y, Rect rect) {
    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
    if (rect.w <= 0 || rect.h <= 0) return;

    for (int ty = 0; ty < rect.h; ty++) {
        std::copy(
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width,
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width + rect.w,
            data.begin() + x + (y + ty) * width);
    }
}

void Texture::blitTransp(const Texture &tex, int x, int y) {
    blitTransp(tex, x, y, Rect(0, 0, tex.width, tex.height));
}

void Texture::blitTransp(const Texture &tex, int x, int y, Rect rect) {
    boundsCheck(rect, x, y, width, height, tex.width, tex.height);
    if (rect.w <= 0 || rect.h <= 0) return;

    uint8_t tranpColor = tex.transparent_color;
    for (int ty = 0; ty < rect.h; ty++) {
        for (int tx = 0; tx < rect.w; tx++) {
            uint8_t color = tex.data[rect.x + tx + (rect.y + ty) * tex.width];
            if (color == tranpColor) continue;
            data[x + tx + (y + ty) * width] = color;
        }
    }
}

void Texture::copy(const std::vector<uint8_t> &other, int offset) {
    std::copy(other.begin(), other.end(), data.begin() + offset);
}

void Texture::copy(const Texture &other) {
    std::copy(other.data.begin(), other.data.end(), data.begin());
}

int readU8(std::ifstream &file) {
    uint8_t data;
    file.read((char *)&data, 1);
    return data;
}

int readU32(std::ifstream &file) {
    uint32_t data;
    file.read((char *)&data, 4);
    return data;
}

int readI16(std::ifstream &file) {
    int16_t data;
    file.read((char *)&data, 2);
    return data;
}

void TexturePack::load(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    // Palette
    size_t paletteSize = readU8(file);
    palette.resize(paletteSize);
    paletteOffset = readU8(file);
    file.read((char *)palette.data(), sizeof(Color) * paletteSize);

    // Textures
    size_t textureCount = readU32(file);
    textures.reserve(textureCount);
    for (int i = 0; i < textureCount; i++) {
        char name[16];
        file.read(name, 16);
        names[name] = i;

        int width = readU32(file);
        int height = readU32(file);
        Texture &newTex = textures.emplace_back(width, height);
        newTex.transparent_color = readI16(file);
        file.read((char *)newTex.data.data(), newTex.width * newTex.height);
    }
}

Texture &TexturePack::getTexture(std::string name) {
    auto index = names.find(name);
    return index == names.end() ? textures[0] : textures[index->second];
}

Texture &TexturePack::getTexture(int id) {
    if (id > textures.size()) {
        return textures[0];
    }
    return textures[id];
}

int TexturePack::getTextureId(std::string name) {
    auto index = names.find(name);
    return index == names.end() ? -1 : index->second;
}