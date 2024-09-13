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

        glm::vec2 invSize{1.0f / width(), 1.0f / height()};
        glm::vec2 rtl{       mx * invSize.x,        my * invSize.y};
        glm::vec2 rtr{1.0f - mx * invSize.x,        my * invSize.y};
        glm::vec2 rbl{       mx * invSize.x, 1.0f - my * invSize.y};
        glm::vec2 rbr{1.0f - mx * invSize.x, 1.0f - my * invSize.y};


        // glm::vec2 rtr{rx - mx * rx,      my * ry};
        // glm::vec2 rbl{     mx * rx, ry - my * ry};
        // glm::vec2 rbr{rx - mx * rx, ry - my * ry};

        quad.setBorderTL(rtl);
        quad.setBorderTR(rtr);
        quad.setBorderBL(rbl);
        quad.setBorderBR(rbr);

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
        quad.draw(quadShader);

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