#pragma once

#include <GLFW/glfw3.h>

#include <string>

/// @brief Class to handle an application
class Application {
public:
    Application();

    /// @brief Constructor
    Application(const std::string &title, const int width, const int height);

    /// @brief Destructor
    ~Application();

    /// @brief Starts app
    void start();

    /// @brief Sets window title
    /// @param title new title
    void setTitle(const std::string &title);

    /// @brief Gets current window width
    /// @return width in pixels
    int width();

    /// @brief Sets new window width
    /// @param width width in pixels
    void setWidth(const int width);

    /// @brief Gets current window height
    /// @return height in pixels
    int height();

    /// @brief Sets new window height
    /// @param height height in pixels
    void setHeight(const int height);

private:
    /// @brief Callback for input processing
    /// @param window
    void processInput();

    /// @brief GLFW window
    GLFWwindow *window;

    /// @brief Window title
    std::string title;
};