#include <iostream>
#include <sstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"
#include "stb/stb_image.h"

#include "application.hpp"
#include "shader.hpp"
#include "quad.hpp"
#include "debug.hpp"
#include "font.hpp"

#ifndef PROJECT_ROOT_FOLDER
#define PROJECT_ROOT_FOLDER "."
#endif

class App : public Application {
public:
    App(std::string text = "");

    void start() override;
    void framebufferSizeCallback(int width, int height) override;

    std::vector<std::shared_ptr<Quad>> quads;

    std::string text;
};

void App::framebufferSizeCallback(int width, int height) {
    Application::framebufferSizeCallback(width, height);

    glm::vec2 windowSize{(float)width, (float)height};
    for (auto &quad : quads) {
        quad->onWindowResize(windowSize);
    }
}

App::App(std::string text) : text{text} {
    if (App::text.empty()) {
        App::text = "Hello world!";
    }
}

void App::start() {
    std::string rootPath = PROJECT_ROOT_FOLDER;

    if (!initFonts()) {
        throw std::runtime_error{"Failed to initialize fonts"};
    }

    Font minecraftFont{rootPath + "/resources/fonts/minecraft.ttf"};

    // Set polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glCheckError();

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();
    glEnable(GL_BLEND); glCheckError();

    // Render loop
    // -----------
    float before = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Get dt
        float now = glfwGetTime();
        float dt = now - before;
        before = now;

        // Input
        // -----
        processInput();

        // Update title
        std::stringstream sstr;
        sstr << "Rounded Quads | " << (int)(1 / dt) << " fps";
        setTitle(sstr.str().c_str());

        // Rendering commands
        // ------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glCheckError();
        glClear(GL_COLOR_BUFFER_BIT); glCheckError();

        // Swap buffers and poll events
        // ----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminateFonts();
}

int main(int argc, char **argv) {
    std::string text;
    if (argc == 2)
        text = argv[1];
    auto app = std::make_shared<App>(text);
    app->start();
}