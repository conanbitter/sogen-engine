#include "grami/grami.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include <utility>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 270;
const int SCREEN_SCALE = 3;

void Log(const char* line) {
    std::cout << line << std::endl;
}

class TestScene : public Scene {
    int x;
    int y;
    int dx;
    int dy;
    int w;
    int h;
    int back;
    int front;
    Texture bg;
    Texture fg;

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        x = rand() % SCREEN_WIDTH;
        y = rand() % SCREEN_HEIGHT;
        dx = rand() % 2 == 0 ? 1 : -1;
        dy = rand() % 2 == 0 ? 1 : -1;

        bg = Texture("../../../../assets/bg2.jpg");
        fg = Texture("../../../../assets/logo.png");
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        x += dx;
        y += dy;
        if (x >= SCREEN_WIDTH - fg.getWidth() + 20) dx = -1;
        if (y >= SCREEN_HEIGHT - fg.getHeight() + 20) dy = -1;
        if (x <= -20) dx = 1;
        if (y <= -20) dy = 1;
    }

    void onRender(App& app, Renderer& gfx) override {
        // gfx.clear(Color::Black);
        gfx.copy(bg);
        gfx.blitAlpha(fg, x, y, 0.5f);
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