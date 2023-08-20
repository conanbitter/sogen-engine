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

static void adjustRects(Rect &srcRect, int srcWidth, int srcHeight, int &dstX, int &dstY, int dstWidth, int dstHeight) {
    Rect srcTexRect = Rect(0, 0, srcWidth, srcHeight);
    Rect dstTexRect = Rect(0, 0, dstWidth, dstHeight);

    Rect newSrcRect = srcRect.crop(srcTexRect);
    newSrcRect = newSrcRect.crop(dstTexRect.offset(srcRect.x - dstX, srcRect.y - dstY));

    dstX += newSrcRect.x - srcRect.x;
    dstY += newSrcRect.y - srcRect.y;
    srcRect = newSrcRect;
}

void Texture::blit(const Texture &tex, int x, int y, Rect rect) {
    adjustRects(rect, tex.width, tex.height, x, y, width, height);
    if (rect.w <= 0 || rect.h <= 0) return;

    for (int ty = 0; ty < rect.h; ty++) {
        std::copy(
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width,
            tex.data.begin() + rect.x + (rect.y + ty) * tex.width + rect.w,
            data.begin() + x + (y + ty) * width);
    }
}

void Texture::blitAlpha(const Texture &tex, int x, int y, float alpha, bool isPremultiplied) {
    blitAlpha(tex, x, y, Rect(0, 0, tex.width, tex.height), alpha, isPremultiplied);
}

void Texture::blitAlpha(const Texture &tex, int x, int y, Rect rect, float alpha, bool isPremultiplied) {
    adjustRects(rect, tex.width, tex.height, x, y, width, height);
    if (rect.w <= 0 || rect.h <= 0) return;
    for (int ty = 0; ty < rect.h; ty++) {
        for (int tx = 0; tx < rect.w; tx++) {
            Color fg = tex.data[rect.x + tx + (rect.y + ty) * tex.width];
            if (fg.a == 0) continue;
            float pixelAlpha = (float)fg.a / 255.0f * alpha;
            Color &bg = data[x + tx + (y + ty) * width];
            if (isPremultiplied) {
                bg = fg + bg * (1 - pixelAlpha);
            } else {
                bg = fg * pixelAlpha + bg * (1 - pixelAlpha);
            }
        }
    }
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

static int min(int v1, int v2) { return v1 < v2 ? v1 : v2; }
static int max(int v1, int v2) { return v1 > v2 ? v1 : v2; }

Rect Rect::crop(const Rect other) {
    int x1r = x;
    int x2r = x + w;
    int y1r = y;
    int y2r = y + h;

    int x1o = other.x;
    int x2o = other.x + other.w;
    int y1o = other.y;
    int y2o = other.y + other.h;

    int x1 = max(x1r, x1o);
    int x2 = min(x2r, x2o);
    int y1 = max(y1r, y1o);
    int y2 = min(y2r, y2o);

    return Rect(x1, y1, x2 - x1, y2 - y1);
}

Rect Rect::offset(int dx, int dy) {
    return Rect(x + dx, y + dy, w, h);
}

uint8_t clampColor(int color) {
    if (color > 255) return 255;
    if (color < 0) return 0;
    return color;
}

Color Color::operator+(const Color &other) {
    return Color(
        clampColor(r + other.r),
        clampColor(g + other.g),
        clampColor(b + other.b),
        clampColor(a + other.a));
}

Color &Color::operator+=(const Color &other) {
    r = clampColor(r + other.r);
    g = clampColor(g + other.g);
    b = clampColor(b + other.b);
    a = clampColor(a + other.a);
    return *this;
}

Color Color::operator-(const Color &other) {
    return Color(
        clampColor(r - other.r),
        clampColor(g - other.g),
        clampColor(b - other.b),
        clampColor(a - other.a));
}

Color &Color::operator-=(const Color &other) {
    r = clampColor(r - other.r);
    g = clampColor(g - other.g);
    b = clampColor(b - other.b);
    a = clampColor(a - other.a);
    return *this;
}

Color Color::operator*(float k) {
    return Color(
        clampColor((float)r * k),
        clampColor((float)g * k),
        clampColor((float)b * k),
        clampColor((float)a * k));
}

Color &Color::operator*=(float k) {
    r = clampColor((float)r * k);
    g = clampColor((float)g * k);
    b = clampColor((float)b * k);
    a = clampColor((float)a * k);
    return *this;
}

Color Color::operator/(float k) {
    return Color(
        clampColor((float)r / k),
        clampColor((float)g / k),
        clampColor((float)b / k),
        clampColor((float)a / k));
}

Color &Color::operator/=(float k) {
    r = clampColor((float)r / k);
    g = clampColor((float)g / k);
    b = clampColor((float)b / k);
    a = clampColor((float)a / k);
    return *this;
}