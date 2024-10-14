#pragma once

#include <glm/glm.hpp>

#ifdef DEBUG

#include <string>

#include "glad/glad.h"

/// @brief Prints a debug-formatted string
/// @tparam ...Args format template types
/// @param format format string
/// @param ...args format arguments
template <typename... Args>
void debugPrint(const char *format, Args &&...args) {
    std::string new_format = std::string{"\033[0;93mDEBUG:\033[0m "} + format;
    printf(new_format.c_str(), std::forward<Args>(args)...);
}

/// @brief Checks OpenGL errors
/// @param file file name
/// @param line file line
void glCheckError_(const char *file, int line);

/// @brief Macro for checking OpenGL errors
#define glCheckError() glCheckError_(__FILE__, __LINE__)

#else

/// @brief Prints a debug-formatted string
/// @tparam ...Args format template types
/// @param format format string
/// @param ...args format arguments
template <typename... Args>
void debugPrint([[maybe_unused]] const char *format, [[maybe_unused]] Args &&...args) {}

/// @brief Macro for checking OpenGL errors
#define glCheckError()

#endif // DEBUG

/// @brief Prints a 4x4 matrix
/// @param m matrix to be printed
void printMat4(const glm::mat4 &m);