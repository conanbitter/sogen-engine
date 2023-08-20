#include "grami/grami.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <utility>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
const int SCREEN_SCALE = 3;

void Log(const char* line) {
    std::cout << line << std::endl;
}

std::vector<Color> pal{
    {0, 0, 0},
    {255, 200, 100}};

class TestScene : public Scene {
    int x;
    int y;
    int dx;
    int dy;
    int w;
    int h;
    int back;
    int front;

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        /*texs.load("../../../../assets/test.txs");
        back = texs.getTextureId("poster 3");
        front = texs.getTextureId("avatar1");
        gfx.setPalette(pal);
        gfx.setPalette(texs);
        gfx.clear(0);
        w = texs.getTexture(front).getWidth();
        h = texs.getTexture(front).getHeight();

        x = rand() % (SCREEN_WIDTH - w);
        y = rand() % (SCREEN_HEIGHT - h);
        dx = rand() % 2 == 0 ? 1 : -1;
        dy = rand() % 2 == 0 ? 1 : -1;

        Texture& head = texs.getTexture(front);
        texs.getTexture(back).blitTransp(head, 40, 40);
        for (int i = 0; i < head.getWidth() * head.getHeight(); i += 2) {
            head[i] = head.getTransparent();
        }*/
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        /*x += dx;
        y += dy;
        if (x + w >= SCREEN_WIDTH) dx = -1;
        if (y + h >= SCREEN_HEIGHT) dy = -1;
        if (x <= 0) dx = 1;
        if (y <= 0) dy = 1;*/
    }

    void onRender(App& app, Renderer& gfx) override{
        /*Texture& head = texs.getTexture(front);
        for (int i = 0; i < head.getWidth() * head.getHeight(); i++) {
            int tx = rand() % head.getWidth();
            int ty = rand() % head.getHeight();
            if (head[std::pair<int, int>(tx, ty)] == head.getTransparent()) continue;
            head[std::pair<int, int>(tx, ty)] = head.getTransparent();
            break;
        }*/
        /*gfx.blit(texs.getTexture(back));
        gfx.blitTransp(texs.getTexture(front), x, y);
        texs.getTexture(back).copy(gfx);*/
    };
};

int main() {
    std::cout << std::filesystem::current_path() << std::endl;
    TestScene scene;
    App& app = App::getInstance();
    app.init("SOGEN", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    app.setScene(&scene);
    app.run();
}