#include <iostream>
#include <sstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"
#include "shader.hpp"
#include "quad.hpp"

class App : public Application {
public:
    void start() override;
};

void App::start() {
    std::string rootPath = PROJECT_ROOT_FOLDER;
    // Quad shader
    Shader quadShader{
        rootPath + "/" + "./shaders/quad_vert_shader.glsl",
        rootPath + "/" + "./shaders/quad_frag_shader.glsl"
    };

    auto quad = std::make_shared<Quad>(glm::vec2{.0f}, glm::vec2{0.5f});
    auto quad1 = std::make_shared<Quad>(glm::vec2{-1.0f + 0.033f, 1.0f - 0.033f}, glm::vec2{0.5f});

    quad->setColor(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    quad->setBorderRadius(BorderRadius::all(
        Radius::circularPixel(75)
    ));
    quad->addChild(quad1);

    quad1->setColor(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
    quad1->setBorderRadius(BorderRadius::all(
        Radius::circularPixel(15)
    ));
    quad1->setAnchorPoint(glm::vec2{0.0f, 1.0f});

    // Set polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Render loop
    // -----------
    float before = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
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

        auto rtl = Radius::ellipticalScale(       mx,        my);
        auto rtr = Radius::ellipticalScale(1.0f - mx,        my);
        auto rbl = Radius::ellipticalScale(       mx, 1.0f - my);
        auto rbr = Radius::ellipticalScale(1.0f - mx, 1.0f - my);

        quad1->setBorderRadius(BorderRadius(
            rbr,
            rbl,
            rtr,
            rtl
        ));

        quad1->setSize(glm::vec2{mx, my});
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw quad
        glm::vec2 windowSize{width(), height()};
        quad->draw(quadShader, windowSize);

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