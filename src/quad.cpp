#include <glm/glm.hpp>

#include "include/quad.hpp"

// Default vertices for quad
const float __vertices[] = {
    // positions
    -1.0f, -1.0f, // bottom left
    -1.0f,  1.0f, // top left
     1.0f,  1.0f, // top right
     1.0f, -1.0f, // bottom right
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
) : _pos{pos}, _size{size}, _color{color} {
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Quad::setPosition(const glm::vec2 &pos) {
    _pos = pos;
}

glm::vec2 Quad::position() const {
    return _pos;
}

void Quad::setSize(const glm::vec2 &size) {
    _size = size;
}

glm::vec2 Quad::size() const {
    return _size;
}

void Quad::setRotation(float rotation) {
    _rotation = rotation;
}

float Quad::rotation() const {
    return _rotation;
}

void Quad::setBorderRadius(const BorderRadius &borderRadius) {
    _borderRadius = borderRadius;
}

BorderRadius Quad::borderRadius() const {
    return _borderRadius;
}

void Quad::draw(const Shader &shader, const glm::vec2 &windowSize) {
    // Set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
    model = glm::rotate(model, _rotation, glm::vec3{0.0f, 0.0f, 1.0f});
    model = glm::scale(model, glm::vec3(_size.x, _size.y, 1.0f));
    shader.setMat4("model", model);

    // Set attributes
    shader.setVec4("color", _color);
    shader.setVec2("pos", _pos);
    shader.setVec2("size", _size);

    // Get border radius in [0-1] scale
    glm::vec2 quadPixelsSize = windowSize * _size;
    quadPixelsSize.y *= windowSize.x / windowSize.y;    // correct aspect ratio
    BorderRadius scaled = _borderRadius.toScale(quadPixelsSize);

    shader.setVec2("borderTL", scaled.topLeft().    toVector2());
    shader.setVec2("borderTR", scaled.topRight().   toVector2());
    shader.setVec2("borderBL", scaled.bottomLeft(). toVector2());
    shader.setVec2("borderBR", scaled.bottomRight().toVector2());

    // Draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}