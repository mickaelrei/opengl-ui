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
#include "text.hpp"

#ifndef PROJECT_ROOT_FOLDER
#define PROJECT_ROOT_FOLDER "."
#endif

class App : public Application {
public:
    App();

    void start() override;
    void framebufferSizeCallback(int width, int height) override;
    void charCallback(unsigned int codepoint) override;
    void keyCallback(int key, int scancode, int action, int mods) override;

    std::vector<std::shared_ptr<Quad>> quads;

    std::string text;
};

App::App() : Application::Application{PROJECT_ROOT_FOLDER, "Rounded Quads", 600, 600} {}

void App::framebufferSizeCallback(int width, int height) {
    Application::framebufferSizeCallback(width, height);

    // Call window resize callback for each quad
    glm::vec2 windowSize{(float)width, (float)height};
    for (auto &quad : quads) {
        quad->onWindowResize(windowSize);
    }
}

void App::charCallback(unsigned int codepoint) {
    if (codepoint < CHARS_START || codepoint > CHARS_START + CHARS_LEN) return;

    text += (char)codepoint;
}

void App::keyCallback(int key, int scancode, int action, int mods) {
    (void)scancode;

    bool ctrlPressed = false;
    if (mods & GLFW_MOD_CONTROL) {
        ctrlPressed = true;
    }

    if (ctrlPressed && action == GLFW_PRESS && key == GLFW_KEY_V) {
        auto str = glfwGetClipboardString(window);
        printf("clipboard str: \"%s\"\n", str);

        text += str;
        return;
    }

    // If key is backspace and action is not release, remove last char
    if (action != GLFW_RELEASE && key == GLFW_KEY_BACKSPACE) {
        size_t size;
        // If CTRL was pressed delete whole word or until last space
        if (ctrlPressed) {
            auto idx = text.rfind(' ');
            if (idx == -1UL) {
                size = 0;
            } else {
                size = idx;
            }
        } else {
            size = text.size() - 1;
        }
        text = text.substr(0, size);
    }
}


void App::start() {
    // Set polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glCheckError();

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();
    glEnable(GL_BLEND); glCheckError();

    glm::vec2 windowSize{width(), height()};

    // Create quad
    auto quad = std::make_shared<Quad>(windowSize);
    quad->setSize(Dim2::fromScale(0.5f, 0.5f));
    quad->setPosition(Dim2::fromScale(0.5f, 0.5f));
    quad->setAnchorPoint(glm::vec2{0.5f});
    quad->setColor(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    quad->setBorderRadius(
        BorderRadius(Radius::circular(
            Dim::fromScale(0.5f)
        ))
    );
    quads.push_back(quad);

    Font font{"resources/fonts/minecraft.ttf"};
    Text textBox{
        "Most words are short & don't need to break. But Antidisestablishmentarianism is long. The width is set to min-content, with a max-width of 11em. ",
        font
    };
    textBox.setRenderWidth(300.0f);

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

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            setWidth(width() - 1);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            setWidth(width() + 1);
        }

        // Update title
        std::stringstream sstr;
        sstr << "Rounded Quads | " << (int)(1 / dt) << " fps";
        setTitle(sstr.str().c_str());

        windowSize = glm::vec2{(float)width(), (float)height()};

        // Change quad border radius based on mouse position
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        mx /= width();
        my /= height();
        auto rtl = Radius::elliptical(Dim2::fromScale(       mx,        my));
        auto rtr = Radius::elliptical(Dim2::fromScale(1.0f - mx,        my));
        auto rbl = Radius::elliptical(Dim2::fromScale(       mx, 1.0f - my));
        auto rbr = Radius::elliptical(Dim2::fromScale(1.0f - mx, 1.0f - my));
        quad->setBorderRadius(BorderRadius(
            rbr,
            rbl,
            rtr,
            rtl
        ));

        // Rendering commands
        // ------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glCheckError();
        glClear(GL_COLOR_BUFFER_BIT); glCheckError();

        for (auto const &quad : quads) {
            quad->draw(windowSize);
        }

        float r = std::sin(now) * 0.5f + 0.5f;
        float g = std::cos(now * 4.0f) * 0.5f + 0.5f;
        textBox.setColor(glm::vec4{r, g, (r + g) * 0.5f, 1.0f});
        textBox.setRenderWidth((float)width());
        textBox.setText(text);
        textBox.draw(windowSize);

        // Swap buffers and poll events
        // ----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    auto app = std::make_shared<App>();
    app->start();
}