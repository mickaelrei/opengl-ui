#pragma once

#include "include/shader.hpp"

class Quad {
public:
    Quad();

    Quad(
        const glm::vec2 &pos,
        const glm::vec2 &size,
        const glm::vec4 &color = glm::vec4{1.0f}
    );

    void setSize(const glm::vec2 &size);
    void setPosition(const glm::vec2 &pos);

    void setBorderTL(const glm::vec2 &border);
    void setBorderTR(const glm::vec2 &border);
    void setBorderBL(const glm::vec2 &border);
    void setBorderBR(const glm::vec2 &border);

    void draw(const Shader &shader);
private:
    glm::vec2 pos;

    glm::vec2 size;

    glm::vec4 color;

    glm::vec2 borderTL;
    glm::vec2 borderTR;
    glm::vec2 borderBL;
    glm::vec2 borderBR;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};