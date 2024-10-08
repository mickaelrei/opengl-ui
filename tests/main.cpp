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

    Shader textShader{
        rootPath + "/resources/shaders/text.vs",
        rootPath + "/resources/shaders/text.fs"
    };

    Font minecraftFont{rootPath + "/resources/fonts/minecraft.ttf"};

    // Set polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glCheckError();

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();
    glEnable(GL_BLEND); glCheckError();

    // Text quad data
    unsigned int textVAO, textVBO, textEBO;
    {
        // Default vertices for quad
        const float __vertices[] = {
            // positions
            -1.0f, -1.0f, // bottom left
            -1.0f,  1.0f, // top left
            1.0f,  1.0f, // top right
            1.0f, -1.0f, // bottom right
        };

        // Default indices for quad
        const unsigned int __indices[] = {
            0, 1, 2, // first triangle
            0, 2, 3  // second triangle
        };

        // Create vertex objects
        glGenVertexArrays(1, &textVAO); glCheckError();

        glGenBuffers(1, &textVBO); glCheckError();
        glGenBuffers(1, &textEBO); glCheckError();

        // Bind the array (VAO) first
        glBindVertexArray(textVAO); glCheckError();

        // Then bind and set the buffer (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, textVBO); glCheckError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(__vertices), __vertices, GL_STATIC_DRAW); glCheckError();

        // Then bind and set the elements buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO); glCheckError();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(__indices), __indices, GL_STATIC_DRAW); glCheckError();

        // How to interpret the vertex data (layout location on vertex shader)
        // Position attribute
        glEnableVertexAttribArray(0); glCheckError();
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0); glCheckError();

        // Unbind buffers
        glBindVertexArray(0); glCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, 0); glCheckError();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); glCheckError();
    }

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

        glm::mat4 projection = glm::ortho(0.0f, (float)width(), (float)height(), 0.0f, 0.0f, 1.0f);
        glm::mat4 model{1.0f};
        model = glm::translate(model, glm::vec3{300.0f, 300.0f, 0.0f});
        model = glm::scale(model, glm::vec3{50.0f});

        // Rendering commands
        // ------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glCheckError();
        glClear(GL_COLOR_BUFFER_BIT); glCheckError();

        auto charA = minecraftFont.getCharInfo('b');
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, charA.textureID);

        textShader.setInt("tex", 0);
        textShader.setMat4("projection", projection);
        textShader.setMat4("model", model);
        textShader.setVec4("color", glm::vec4{1.0f});
        glBindVertexArray(textVAO); glCheckError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
        glBindVertexArray(0); glCheckError();

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