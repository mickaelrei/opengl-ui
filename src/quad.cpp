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

void Quad::setAnchorPoint(const glm::vec2 &anchorPoint) {
    _anchorPoint = glm::clamp(anchorPoint, glm::vec2{0.0f}, glm::vec2{1.0f});
}

glm::vec2 Quad::anchorPoint() const {
    return _anchorPoint;
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

void Quad::setColor(const glm::vec4 &color) {
    _color = color;
}

glm::vec4 Quad::color() const {
    return _color;
}

void Quad::setBorderRadius(const BorderRadius &borderRadius) {
    _borderRadius = borderRadius;
}

BorderRadius Quad::borderRadius() const {
    return _borderRadius;
}

void Quad::addChild(const std::shared_ptr<Quad> &child) {
    children.push_back(child);
}

void Quad::draw(
    const Shader &shader,
    const glm::vec2 &windowSize,
    const glm::mat4 &model
) const {
    // Get translation based on anchor point
    auto _correctedPos = _pos - _size * (_anchorPoint * 2.0f - 1.0f);

    // Set model matrix
    glm::mat4 myModel(1.0f);
    myModel *= model;
    myModel = glm::translate(myModel, glm::vec3(_correctedPos.x, _correctedPos.y, 0.0f));
    myModel = glm::rotate(myModel, _rotation, glm::vec3{0.0f, 0.0f, 1.0f});
    myModel = glm::scale(myModel, glm::vec3(_size.x, _size.y, 1.0f));
    shader.setMat4("model", myModel);

    // Set attributes
    shader.setVec4("color", _color);
    shader.setVec2("pos", _pos);
    shader.setVec2("size", _size);

    // Get border radius in [0-1] scale
    glm::vec2 quadPixelsSize = windowSize * _size;
    BorderRadius scaled = _borderRadius.toScale(quadPixelsSize);

    // Convert to 2D vector
    glm::vec2 borderTL = scaled.topLeft    ().toVector2();
    glm::vec2 borderTR = scaled.topRight   ().toVector2();
    glm::vec2 borderBL = scaled.bottomLeft ().toVector2();
    glm::vec2 borderBR = scaled.bottomRight().toVector2();

    // Correct radius overlap
    {
        float total;
        float div;

        // Left Y
        total = borderTL.y + borderBL.y;
        if (total > 1.0f) {
            div = 1.0f / total;
            borderTL *= div;
            borderBL *= div;
        }

        // Right Y
        total = borderTR.y + borderBR.y;
        if (total > 1.0f) {
            div = 1.0f / total;
            borderTR *= div;
            borderBR *= div;
        }

        // Top X
        total = borderTL.x + borderTR.x;
        if (total > 1.0f) {
            div = 1.0f / total;
            borderTL *= div;
            borderTR *= div;
        }

        // Bottom X
        total = borderBL.x + borderBR.x;
        if (total > 1.0f) {
            div = 1.0f / total;
            borderBL *= div;
            borderBR *= div;
        }
    }

    // Sanity check border radiuses (clamping to [0-1] range to make sure)
    borderTL = glm::clamp(borderTL, glm::vec2{0.0f}, glm::vec2{1.0f});
    borderTR = glm::clamp(borderTR, glm::vec2{0.0f}, glm::vec2{1.0f});
    borderBL = glm::clamp(borderBL, glm::vec2{0.0f}, glm::vec2{1.0f});
    borderBR = glm::clamp(borderBR, glm::vec2{0.0f}, glm::vec2{1.0f});

    // Cache inverses
    bool checkTL = false;
    bool checkTR = false;
    bool checkBL = false;
    bool checkBR = false;

    if (borderTL.x * borderTL.y > 0) {
        checkTL = true;
        shader.setVec2("inv2TL", 1.0f / glm::pow(borderTL, glm::vec2{2.0f}));
    }
    if (borderTR.x * borderTR.y > 0) {
        checkTR = true;
        shader.setVec2("inv2TR", 1.0f / glm::pow(borderTR, glm::vec2{2.0f}));
    }
    if (borderBL.x * borderBL.y > 0) {
        checkBL = true;
        shader.setVec2("inv2BL", 1.0f / glm::pow(borderBL, glm::vec2{2.0f}));
    }
    if (borderBR.x * borderBR.y > 0) {
        checkBR = true;
        shader.setVec2("inv2BR", 1.0f / glm::pow(borderBR, glm::vec2{2.0f}));
    }

    shader.setBool("checkTL", checkTL);
    shader.setBool("checkTR", checkTR);
    shader.setBool("checkBL", checkBL);
    shader.setBool("checkBR", checkBR);

    shader.setVec2("borderTL", borderTL);
    shader.setVec2("borderTR", borderTR);
    shader.setVec2("borderBL", borderBL);
    shader.setVec2("borderBR", borderBR);

    // Draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Draw children
    for (const auto &child : children) {
        child->draw(shader, windowSize, myModel);
    }
}