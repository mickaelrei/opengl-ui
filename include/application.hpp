#pragma once

#include <GLFW/glfw3.h>

#include <string>

/// @brief Class to handle an application
class Application {
public:
    /// @brief Default constructor
    Application() = default;

    /// @brief Constructor
    /// @param title window title
    /// @param width window initial width
    /// @param width window initial height
    /// @param rootPath project root path
    Application(
        const std::string &rootPath,
        const std::string &title = "",
        int width = 600,
        int height = 600
    );

    /// @brief Destructor
    ~Application();

    /// @brief Starts app
    virtual void start();

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

    /// @brief Callback for window resize
    /// @param width new width
    /// @param height new heigth
    virtual void framebufferSizeCallback(int width, int height);

    /// @brief Callback for key tap
    /// @param key keyboard key
    /// @param scancode platform-specific scancode
    /// @param action key action
    /// @param mods modifier bits
    virtual void keyCallback(int key, int scancode, int action, int mods);

    /// @brief Callback for char input
    /// @param codepoint Character UTF-32 codepoint
    virtual void charCallback(unsigned int codepoint);

protected:
    /// @brief Callback for input processing
    /// @param window
    virtual void processInput();

    /// @brief GLFW window
    GLFWwindow *window;

    /// @brief Window title
    std::string title;
};