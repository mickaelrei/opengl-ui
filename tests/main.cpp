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
    // Quad shader
    Shader quadShader{
        rootPath + "/resources/shaders/quad_vert_shader.glsl",
        rootPath + "/resources/shaders/quad_frag_shader.glsl"
    };

    Shader quadTexShader{
        rootPath + "/resources/shaders/quad_vert_shader.glsl",
        rootPath + "/resources/shaders/quad_tex_frag_shader.glsl"
    };

    glm::vec2 windowSize{width(), height()};
    auto quad = std::make_shared<Quad>(windowSize);
    auto quad1 = std::make_shared<Quad>(windowSize);

    quads.push_back(quad);
    quads.push_back(quad1);

    quad->setSize(Dim2::fromPixels(300, 300));
    // quad->setSize(Dim2::fromScale(0.5f, 0.5f));
    quad->setColor(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    quad->setBorderRadius(BorderRadius::all(
        Radius::circular(Dim::fromPixels(75))
    ));
    // quad->addChild(quad1);

    quad1->setSize(Dim2::fromScale(0.5f, 0.5f));
    quad1->setPosition(Dim2::fromScale(-1.0f, 1.0f));
    quad1->setColor(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
    quad1->setBorderRadius(BorderRadius::all(
        Radius::circular(Dim::fromPixels(15))
    ));
    quad1->setAnchorPoint(glm::vec2{0.0f, 1.0f});

    // Load Bitmap font
    unsigned int fontTexture;
    unsigned int textureVAO, textureVBO, textureEBO;
    {
        stbi_set_flip_vertically_on_load(true);
        int texWidth;
        int texHeight;
        int numChannels;
        std::string texturePath = rootPath + "/resources/images/font.png";
        unsigned char *data = stbi_load(
            texturePath.c_str(),
            &texWidth, &texHeight, &numChannels, 0
        );
        if (data == NULL) {
            throw std::invalid_argument{"Could not load image: " + texturePath};
        }

        // Get image format
        int format;
        switch (numChannels)
        {
        case 4:
            format = GL_RGBA;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 1:
            format = GL_RED;
            break;
        default:
            throw std::invalid_argument{"Could not recognize image format:" + texturePath};
        }

        // Generate texture
        glGenTextures(1, &fontTexture); glCheckError();
        glBindTexture(GL_TEXTURE_2D, fontTexture); glCheckError();
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            texWidth,
            texHeight,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        ); glCheckError();
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();

        stbi_image_free(data);
        stbi_set_flip_vertically_on_load(false);

        float vertices[] = {
            // positions
            -1.0f, -1.0f, // bottom left
            -1.0f,  1.0f, // top left
            1.0f,  1.0f, // top right
            1.0f, -1.0f, // bottom right
        };

        unsigned int indices[] = {
            0, 1, 2, // first triangle
            0, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &textureVAO); glCheckError();

        glGenBuffers(1, &textureVBO); glCheckError();
        glGenBuffers(1, &textureEBO); glCheckError();

        // Bind the array (VAO) first
        glBindVertexArray(textureVAO); glCheckError();

        // Then bind and set the buffer (VBO)
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO); glCheckError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); glCheckError();

        // Then bind and set the elements buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO); glCheckError();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); glCheckError();

        // How to interpret the vertex data (layout location on vertex shader)
        // Position attribute
        glEnableVertexAttribArray(0); glCheckError();
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0); glCheckError();

        // Unbind buffers
        glBindVertexArray(0); glCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, 0); glCheckError();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); glCheckError();
    }

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

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        mx /= width();
        my /= height();

        auto rtl = Radius::elliptical(Dim2::fromScale(       mx,        my));
        auto rtr = Radius::elliptical(Dim2::fromScale(1.0f - mx,        my));
        auto rbl = Radius::elliptical(Dim2::fromScale(       mx, 1.0f - my));
        auto rbr = Radius::elliptical(Dim2::fromScale(1.0f - mx, 1.0f - my));

        quad1->setBorderRadius(BorderRadius(
            rbr,
            rbl,
            rtr,
            rtl
        ));

        quad1->setSize(Dim2::fromScale(mx, my));
        quad->setRotation(now);

        // Set projection matrix
        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        quadShader.setMat4("projection", projection);

        // Update title
        std::stringstream sstr;
        sstr << "Rounded Quads | " << (int)(1 / dt) << " fps";
        setTitle(sstr.str().c_str());

        // Rendering commands
        // ------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glCheckError();
        glClear(GL_COLOR_BUFFER_BIT); glCheckError();

        // Draw quad
        // glm::vec2 windowSize{width(), height()};
        // quad->draw(quadShader, windowSize);

        // Draw text
        const glm::vec2 textScale = glm::vec2{1.0f};
        const glm::vec2 textPos = glm::vec2{-0.75f, 0.0f};
        const glm::vec4 textColor = glm::vec4{mx, my, std::sin(now), 1.0f};
        const float step = 1.0f / 16.0f;

        glActiveTexture(GL_TEXTURE0); glCheckError();
        glBindTexture(GL_TEXTURE_2D, fontTexture);  glCheckError();
        quadTexShader.setVec2("size", glm::vec2{step});
        quadTexShader.setMat4("projection", projection);
        quadTexShader.setVec4("color", textColor);
        quadTexShader.setInt("tex", 0);

        // Iterate characters
        for (size_t i = 0; i < text.length(); ++i) {
            // Find row and column in font texture atlas
            char c = text[i];
            int row = c / 16;
            int col = c % 16;

            // Calculate offset
            glm::vec2 offset{col, 15.0f - row};
            offset *= step;
            offset.x -= step * 0.25f;
            quadTexShader.setVec2("offset", offset);

            // Calculate X position
            float posX = textPos.x + step * textScale.x * i * 1.25f;

            glm::mat4 mat{1.0f};
            mat = glm::translate(mat, glm::vec3{posX, textPos.y, 0.0f});
            mat = glm::scale(mat, glm::vec3{textScale * step, 1.0f});
            quadTexShader.setMat4("model", mat);

            glBindVertexArray(textureVAO); glCheckError();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
            glBindVertexArray(0); glCheckError();
        }

        // Swap buffers and poll events
        // ----------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char **argv) {
    std::string text;
    if (argc == 2)
        text = argv[1];
    auto app = std::make_shared<App>(text);
    app->start();
}