#include <stdio.h>

#include "debug.hpp"

#ifdef DEBUG

void glCheckError_(const char *file, int line) {
    GLenum errorCode;
    std::string error;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        std::string fileBold = "\033[0;00;1m";
        fileBold += file;
        fileBold += "\033[0m";
        error = "\033[0;91m" + error + "\033[0m";
        debugPrint("OpenGL error | %s:%d: %s\n", fileBold.c_str(), line, error.c_str());
    }
}

#endif

void printMat4(const glm::mat4 &m) {
    printf("[%.3f, %.3f, %.3f, %.3f]\n", m[0][0], m[0][1], m[0][2], m[0][3]);
    printf("[%.3f, %.3f, %.3f, %.3f]\n", m[1][0], m[1][1], m[1][2], m[1][3]);
    printf("[%.3f, %.3f, %.3f, %.3f]\n", m[2][0], m[2][1], m[2][2], m[2][3]);
    printf("[%.3f, %.3f, %.3f, %.3f]\n", m[3][0], m[3][1], m[3][2], m[3][3]);
}