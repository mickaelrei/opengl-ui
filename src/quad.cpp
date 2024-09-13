#include <glm/glm.hpp>

#include "include/quad.hpp"

// Default vertices for quad
const float __vertices[] = {
    // positions
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f, 0.0f,  // top left
    1.0f, 1.0f, 0.0f,   // top right
    1.0f, -1.0f, 0.0f,  // bottom right
};

// Default indices for quad
const unsigned int __indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
};

Quad::Quad() {}

Quad::Quad(
    const glm::vec2 &pos,
    const glm::vec2 &size,
    const glm::vec4 &color
) : pos{pos}, size{size}, color{color} {
    // Create vertex objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the array (VAO) first
    glBindVertexArray(VAO);

    // Then bind and set the buffer (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(__vertices), __vertices, GL_STATIC_DRAW);

    // Then bind and set the elements buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(__indices), __indices, GL_STATIC_DRAW);

    // How to interpret the vertex data (layout location on vertex shader)
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Quad::setSize(const glm::vec2 &size) {
    Quad::size = size;
}

void Quad::setPosition(const glm::vec2 &pos) {
    Quad::pos = pos;
}

void Quad::setBorderTL(const glm::vec2 &border) {
    borderTL = border;
}

void Quad::setBorderTR(const glm::vec2 &border) {
    borderTR = border;
}

void Quad::setBorderBL(const glm::vec2 &border) {
    borderBL = border;
}

void Quad::setBorderBR(const glm::vec2 &border) {
    borderBR = border;
}

void Quad::draw(const Shader &shader) {
    shader.use();

    // Set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    shader.setMat4("model", model);

    // Set attributes
    shader.setVec4("color", color);
    shader.setVec2("pos", pos);
    shader.setVec2("size", size);

    shader.setVec2("borderTL", borderTL);
    shader.setVec2("borderTR", borderTR);
    shader.setVec2("borderBL", borderBL);
    shader.setVec2("borderBR", borderBR);

    // Draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}