#include <glm/glm.hpp>

#include "quad.hpp"

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

Quad::Quad(const glm::vec2 &windowSize) {
    onWindowResize(windowSize);

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

void Quad::setPosition(const Dim2 &pos) {
    _pos = pos;
    calculateModelMatrix();
}

Dim2 Quad::position() const {
    return _pos;
}

void Quad::setAnchorPoint(const glm::vec2 &anchorPoint) {
    _anchorPoint = glm::clamp(anchorPoint, glm::vec2{0.0f}, glm::vec2{1.0f});
    calculateModelMatrix();
}

glm::vec2 Quad::anchorPoint() const {
    return _anchorPoint;
}

void Quad::setSize(const Dim2 &size) {
    _size = Dim2::max(Dim2::zero(), size);
    calculateModelMatrix();
}

Dim2 Quad::size() const {
    return _size;
}

void Quad::setRotation(float rotation) {
    _rotation = rotation;
    calculateModelMatrix();
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
) {
    setUniforms(shader, windowSize, model);

    // Draw elements
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Draw children
    for (auto &child : children) {
        child->draw(shader, windowSize, model * _modelMatrix);
    }
}

void Quad::setUniforms(
    const Shader &shader,
    const glm::vec2 &windowSize,
    const glm::mat4 &model
) {
    // // Get translation based on anchor point
    auto _scaledPos = _pos.toScale(windowSize);
    auto _scaledSize = _size.toScale(windowSize);

    // Set attributes
    shader.setVec4("color", _color);
    shader.setVec2("pos", _scaledPos);
    shader.setVec2("size", _scaledSize);
    shader.setMat4("model", model * _modelMatrix);

    // Get border radius in [0-1] scale
    glm::vec2 quadPixelsSize = _size.toPixels(windowSize);

    // Convert to 2D vector
    glm::vec2 borderTL = _borderRadius.topLeft    ().toScale(quadPixelsSize);
    glm::vec2 borderTR = _borderRadius.topRight   ().toScale(quadPixelsSize);
    glm::vec2 borderBL = _borderRadius.bottomLeft ().toScale(quadPixelsSize);
    glm::vec2 borderBR = _borderRadius.bottomRight().toScale(quadPixelsSize);

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
}

void Quad::onWindowResize(const glm::vec2 &windowSize) {
    _lastWindowSize = windowSize;
    calculateModelMatrix();
}

void Quad::calculateModelMatrix() {
    // Get translation based on anchor point
    auto _scaledPos = _pos.toScale(_lastWindowSize);
    auto _scaledSize = _size.toScale(_lastWindowSize);
    auto _correctedPos = _scaledPos - _scaledSize * (_anchorPoint * 2.0f - 1.0f);

    // Set model matrix
    _modelMatrix = glm::mat4{1.0f};
    _modelMatrix = glm::translate(_modelMatrix, glm::vec3(_correctedPos, 0.0f));
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(_scaledSize, 1.0f));
    _modelMatrix = glm::rotate(_modelMatrix, _rotation, glm::vec3{0.0f, 0.0f, 1.0f});
}