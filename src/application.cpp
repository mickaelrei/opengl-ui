#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/glad.h"

#include "include/application.hpp"
#include "include/shader.hpp"
#include "include/quad.hpp"

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

    Quad quad{glm::vec2{.0f}, glm::vec2{0.5f}};
    // quad.setBorderRadius(15);

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

        auto rtl = Radius::ellipticalPixel(          mx,            my);
        auto rtr = Radius::ellipticalPixel(width() - mx,            my);
        auto rbl = Radius::ellipticalPixel(          mx, height() - my);
        auto rbr = Radius::ellipticalPixel(width() - mx, height() - my);

        quad.setBorderRadius(BorderRadius(
            // Radius::circularScale(1.0f),
            // Radius::zero(),
            // Radius::zero(),
            // Radius::circularScale(1.0f)

            rbr,
            rbl,
            rtr,
            rtl

            // rbr,
            // Radius::zero(),
            // Radius::zero(),
            // rtl
        ));
        // quad.setBorderRadius(BorderRadius::circularScale(mx / width()));
        // std::cout << "Quad border radius: " << quad.borderRadius() << "\n";

        // quad.setRotation(now);

        // Update aspect ratio
        float aspectRatio = (float)width() / (float)height();

        // Set projection matrix
        glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
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
        quad.draw(quadShader, windowSize);

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