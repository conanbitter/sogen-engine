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

struct Entity {
    int x;
    int y;
    int dx;
    int dy;
};

class TestScene : public Scene {
    Texture bg;
    Texture fg;
    std::vector<Entity> entities;

    void onLoad(App& app, Renderer& gfx) override {
        Scene::onLoad(app, gfx);
        for (int i = 0; i < 100; i++) {
            Entity data;
            data.x = rand() % SCREEN_WIDTH;
            data.y = rand() % SCREEN_HEIGHT;
            data.dx = rand() % 2 == 0 ? 1 : -1;
            data.dy = rand() % 2 == 0 ? 1 : -1;
            entities.push_back(data);
        }

        bg = Texture("../../../../assets/bg2.jpg");
        fg = Texture("../../../../assets/logo.png");
    }

    void onUnload(App& app, Renderer& gfx) override {
        Scene::onUnload(app, gfx);
    }

    void onUpdate(float deltaTime, App& app, Renderer& gfx) override {
        for (Entity& entity : entities) {
            entity.x += entity.dx;
            entity.y += entity.dy;
            if (entity.x >= SCREEN_WIDTH - fg.getWidth() + 20) entity.dx = -1;
            if (entity.y >= SCREEN_HEIGHT - fg.getHeight() + 20) entity.dy = -1;
            if (entity.x <= -20) entity.dx = 1;
            if (entity.y <= -20) entity.dy = 1;
        }
    }

    void onRender(App& app, Renderer& gfx) override {
        // gfx.clear(Color::Black);
        gfx.copy(bg);
        for (Entity& entity : entities) {
            gfx.blitAlpha(fg, entity.x, entity.y, 0.5f);
        }
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