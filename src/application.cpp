#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad.h"

#include "application.hpp"
#include "shader.hpp"
#include "quad.hpp"

// Window resize
static void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

Application::Application() : Application{"", 600, 600} {}

Application::Application(const std::string &title, const int width, const int height)
    : title{title}
{
    // Init GLFW
    // ---------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    // -------------
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // Load GLAD
    // ---------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        exit(-1);
    }
}

Application::~Application()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::start()
{
    // Quad shader
    Shader quadShader{"./shaders/quad_vert_shader.glsl", "./shaders/quad_frag_shader.glsl"};

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

        // auto rtl = Radius::ellipticalScale(       mx,        my);
        // auto rtr = Radius::ellipticalScale(1.0f - mx,        my);
        // auto rbl = Radius::ellipticalScale(       mx, 1.0f - my);
        // auto rbr = Radius::ellipticalScale(1.0f - mx, 1.0f - my);

        // quad1->setBorderRadius(BorderRadius(
        //     rbr,
        //     rbl,
        //     rtr,
        //     rtl
        // ));

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

void Application::setTitle(const std::string &title)
{
    Application::title = title;
    glfwSetWindowTitle(window, Application::title.c_str());
}

void Application::setWidth(const int width)
{
    glfwSetWindowSize(window, width, height());
}

int Application::width()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

void Application::setHeight(const int height)
{
    glfwSetWindowSize(window, width(), height);
}

int Application::height()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

void Application::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}