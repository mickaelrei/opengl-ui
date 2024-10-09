#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

/// @brief Wrapper class for an OpenGL shader
class Shader {
public:
    /// @brief Default constructor
    Shader() = default;

    /// @brief Constructor with file paths
    /// @param vertexPath path to vertex shader
    /// @param fragmentPath path to fragment shader
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    /// @brief Use/activate shader
    void use() const;

    /// @brief Destroy shader
    void destroy() const;

    /// @brief Set uniform bool
    /// @param name uniform name
    /// @param value bool
    void setBool(const std::string &name, bool value) const;

    /// @brief Set uniform integer
    /// @param name uniform name
    /// @param value integer
    void setInt(const std::string &name, int value) const;

    /// @brief Set uniform float
    /// @param name uniform name
    /// @param value float
    void setFloat(const std::string &name, float value) const;

    /// @brief Set uniform vec2
    /// @param name uniform name
    /// @param value vec2
    void setVec2(const std::string &name, const glm::vec2 &value) const;

    /// @brief Set uniform vec3
    /// @param name uniform name
    /// @param value vec3
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    /// @brief Set uniform vec4
    /// @param name uniform name
    /// @param value vec4
    void setVec4(const std::string &name, const glm::vec4 &value) const;

    /// @brief Set uniform mat4
    /// @param name uniform name
    /// @param value mat4
    void setMat4(const std::string &name, const glm::mat4 &value) const;

    /// @brief OpenGL identifier
    unsigned int id;
};