#include <iostream>

#include "glad/glad.h"

#include "application.hpp"
#include "font.hpp"
#include "quad.hpp"
#include "text.hpp"
#include "debug.hpp"

// Window resize
static void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    Application *app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->framebufferSizeCallback(width, height);
}

static void glfwErrorCallback(int errorCode, const char* description) {
    std::cerr << "GLFW Error;\n";
    std::cerr << "Error code: " << errorCode << "\n";
    std::cerr << "Description: " << description << "\n";
}

Application::Application(
    const std::string &rootPath,
    const std::string &title,
    int width,
    int height
)
    : title{title} {
    glfwSetErrorCallback(glfwErrorCallback);

    // Init GLFW
    // ---------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    // -------------
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetWindowUserPointer(window, this);

    // Load GLAD
    // ---------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        exit(1);
    }

    // Init modules
    // ------------
    glm::vec2 windowSize{(float)width, (float)height};
    if (!FontModule::init(rootPath)) {
        std::cout << "Failed to initialize font module\n";
        exit(1);
    }

    if (!TextModule::init(rootPath, windowSize)) {
        std::cout << "Failed to initialize text module\n";
        exit(1);
    }

    if (!QuadModule::init(rootPath, windowSize)) {
        std::cout << "Failed to initialize quad module\n";
        exit(1);
    }
}

Application::~Application() {
    FontModule::terminate();
    TextModule::terminate();
    QuadModule::terminate();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::start() {}

void Application::setTitle(const std::string &title) {
    Application::title = title;
    glfwSetWindowTitle(window, Application::title.c_str());
}

void Application::setWidth(const int width) {
    glfwSetWindowSize(window, width, height());
}

int Application::width() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

void Application::setHeight(const int height) {
    glfwSetWindowSize(window, width(), height);
}

int Application::height() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

void Application::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::framebufferSizeCallback(int width, int height) {
    glViewport(0, 0, width, height); glCheckError();

    // Call modules window resize callback
    glm::vec2 windowSize{(float)width, (float)height};
    TextModule::onWindowResize(windowSize);
    QuadModule::onWindowResize(windowSize);
}