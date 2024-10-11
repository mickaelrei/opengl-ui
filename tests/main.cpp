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
    App();

    void start() override;
    void framebufferSizeCallback(int width, int height) override;

    void renderText(
        const std::string &text,
        const glm::vec2 &baseline,
        float fontSize = 14.0f,
        float lineHeight = 1.2f,
        const glm::vec4 &color = glm::vec4{1.0f}
    );

    std::vector<std::shared_ptr<Quad>> quads;

    Font font;
    Shader textShader;
    unsigned int textVAO;
};

App::App() : Application::Application{"Rounded Quads", 300, 700} {}

void App::framebufferSizeCallback(int width, int height) {
    Application::framebufferSizeCallback(width, height);

    glm::vec2 windowSize{(float)width, (float)height};
    for (auto &quad : quads) {
        quad->onWindowResize(windowSize);
    }
}

void App::renderText(
    const std::string &text,
    const glm::vec2 &baseline,
    float fontSize,
    float lineHeight,
    const glm::vec4 &color
) {
    // Get projection
    auto projection = glm::ortho(0.0f, (float)width(), (float)height(), 0.0f, 0.0f, 1.0f);
    glm::mat4 model{1.0f};

    // Set base uniforms
    textShader.setMat4("projection", projection);
    textShader.setVec4("color", color);
    textShader.setInt("tex", 0);

    // Base GL bindings
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO); glCheckError();

    // Calculate font scale based on given font size and font loaded height
    float scale = fontSize / font.fontHeight();

    const float renderWidth = (float)width() - 5.0f;

    // Keep track of current render position
    const float fontOffsetY = font.maxCharHeight() - font.maxCharUnderflow();
    float x = baseline.x;
    float y = baseline.y;

    bool startOfWord = true;
    const size_t textSize = text.size();
    for (size_t i = 0; i < textSize; ++i) {
        char c = text[i];
        auto charData = font.getCharInfo(c);

        // Skip space char
        if (c == ' ') {
            x += charData.advance * scale;
            startOfWord = true;
            continue;
        }

        // Check if next word overflows width
        if (startOfWord && i != 0) {
            // Find next space
            size_t idx = text.find(' ', i);

            // If not found, use end of string
            if (idx == -1UL) {
                idx = textSize;
            }
            auto word = text.substr(i, idx - i);

            // Found next space, check if word is too large
            float textWidth = font.calculateTextWidth(word, fontSize);
            if (-baseline.x + x + textWidth > renderWidth) {
                // Skip line and reset x
                x = baseline.x;
                y += fontSize * lineHeight;
            }
        } else if (i != 0 && -baseline.x + x + charData.advance * scale > renderWidth) {
            // If not start of word, only skip line if current character overflows width
            x = baseline.x;
            y += fontSize * lineHeight;
        }

        // Reset word flag
        startOfWord = false;


        glBindTexture(GL_TEXTURE_2D, charData.textureID);

        // Calculate offset
        float xpos = x + charData.bearing.x * scale;
        float ypos = y + (fontOffsetY - charData.bearing.y) * scale;


        // Calculate new model matrix
        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3{xpos, ypos, 0.0f});
        model = glm::scale(model, glm::vec3{charData.size * scale, 1.0f});

        // Change render position
        x += charData.advance * scale;

        textShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
    }

    // Unbind text VAO
    glBindVertexArray(0); glCheckError();
}

void App::start() {
    std::string rootPath = PROJECT_ROOT_FOLDER;

    if (!initFonts()) {
        throw std::runtime_error{"Failed to initialize fonts"};
    }

    textShader = Shader{
        rootPath + "/resources/shaders/text.vs",
        rootPath + "/resources/shaders/text.fs"
    };

    font = Font{rootPath + "/resources/fonts/roboto.ttf"};

    // Set polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glCheckError();

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();
    glEnable(GL_BLEND); glCheckError();

    // Text quad data
    unsigned int textVBO, textEBO;
    {
        // Default vertices for quad
        const float __vertices[] = {
            // positions
            0.0f, 0.0f, // bottom left
            0.0f, 1.0f, // top left
            1.0f, 1.0f, // top right
            1.0f, 0.0f, // bottom right
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

        // Rendering commands
        // ------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glCheckError();
        glClear(GL_COLOR_BUFFER_BIT); glCheckError();

        char i = ' ' + (int)(now * 5.0f) % 95;
        std::string str{i};

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        glm::vec2 mouseOffset{0.0f, 0.0f};//{mx, my};

        renderText(
            // "This is a paragraph with a standard line-height. Thisisaverylongword",
            "Most words are short & don't need to break. But Antidisestablishmentarianism is long. The width is set to min-content, with a max-width of 11em. ",
            mouseOffset + glm::vec2{5.0f, 5.0f},
            38.0f
        );
        renderText(
            "Testing strings",
            mouseOffset + glm::vec2{5.0f, (float)height() - 38.0f - 5.0f},
            38.0f
        );

        // Swap buffers and poll events
        // ----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminateFonts();
}

int main() {
    auto app = std::make_shared<App>();
    app->start();
}