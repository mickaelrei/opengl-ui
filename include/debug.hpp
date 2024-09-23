#pragma once

#ifdef DEBUG

#include <string>

#include "glad/glad.h"

template <typename... Args>
void debugPrint(const char *format, Args &&...args) {
    std::string new_format = std::string{"\033[0;93mDEBUG:\033[0m "} + format;
    printf(new_format.c_str(), std::forward<Args>(args)...);
}

void glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#else

template <typename... Args>
void debugPrint([[maybe_unused]] const char *format, [[maybe_unused]] Args &&...args) {}

#define glCheckError()

#endif // DEBUG