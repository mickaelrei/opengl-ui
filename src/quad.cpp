#include <glm/glm.hpp>

#include "quad.hpp"
#include "debug.hpp"

/// @brief Shader used to render quads
static Shader quadShader;

/// @brief OpenGL objects for quad rendering
static unsigned int quadVAO, quadVBO, quadEBO;

/// @brief Whether quad resources are already initialized
static bool initialized = false;

namespace QuadModule {

bool init(const std::string &rootPath, const glm::vec2 &windowSize) {
    if (initialized) return true;
    initialized = true;


    // Initialize shader and set initial uniforms
    // ------------------------------------------
    quadShader = Shader{
        rootPath + "/resources/shaders/quad.vs",
        rootPath + "/resources/shaders/quad.fs"
    };
    onWindowResize(windowSize);

    // Construct VAO for text rendering
    // --------------------------------

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

    // Create vertex objects
    glGenVertexArrays(1, &quadVAO); glCheckError();

    glGenBuffers(1, &quadVBO); glCheckError();
    glGenBuffers(1, &quadEBO); glCheckError();

    // Bind the array (VAO) first
    glBindVertexArray(quadVAO); glCheckError();

    // Then bind and set the buffer (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO); glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(__vertices), __vertices, GL_STATIC_DRAW); glCheckError();

    // Then bind and set the elements buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO); glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(__indices), __indices, GL_STATIC_DRAW); glCheckError();

    // How to interpret the vertex data (layout location on vertex shader)
    // Position attribute
    glEnableVertexAttribArray(0); glCheckError();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0); glCheckError();

    // Unbind buffers
    glBindVertexArray(0); glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0); glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); glCheckError();

    // Adding an empty string at the end to suppress compiler warning
    debugPrint("Quad module successfully loaded\n%s", "");
    return true;
}

void terminate() {
    if (!initialized) return;
    initialized = false;

    quadShader.destroy();
    glDeleteBuffers(1, &quadVBO); glCheckError();
    glDeleteBuffers(1, &quadEBO); glCheckError();
    glDeleteVertexArrays(1, &quadVAO); glCheckError();
}

void onWindowResize(const glm::vec2 &windowSize) {
    if (!initialized) return;

    quadShader.setMat4("projection", glm::ortho(
        // left-right
        0.0f, windowSize.x,

        // top-bottom
        windowSize.y, 0.0f,

        // near-far
        0.0f, 1.0f
    ));
}

}

Quad::Quad(const glm::vec2 &windowSize) {
    onWindowResize(windowSize);
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
    const glm::vec2 &windowSize,
    const glm::mat4 &model
) {
    setUniforms(windowSize, model);

    // Draw elements
    glBindVertexArray(quadVAO); glCheckError();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
    glBindVertexArray(0); glCheckError();

    // Draw children
    for (auto &child : children) {
        child->draw(windowSize, model * _modelMatrix);
    }

    glBindVertexArray(0); glCheckError();
}

void Quad::setUniforms(
    const glm::vec2 &windowSize,
    const glm::mat4 &model
) {
    // Set attributes
    quadShader.setVec4("color", _color);
    quadShader.setMat4("model", model * _modelMatrix);

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
        quadShader.setVec2("inv2TL", 1.0f / glm::pow(borderTL, glm::vec2{2.0f}));
    }
    if (borderTR.x * borderTR.y > 0) {
        checkTR = true;
        quadShader.setVec2("inv2TR", 1.0f / glm::pow(borderTR, glm::vec2{2.0f}));
    }
    if (borderBL.x * borderBL.y > 0) {
        checkBL = true;
        quadShader.setVec2("inv2BL", 1.0f / glm::pow(borderBL, glm::vec2{2.0f}));
    }
    if (borderBR.x * borderBR.y > 0) {
        checkBR = true;
        quadShader.setVec2("inv2BR", 1.0f / glm::pow(borderBR, glm::vec2{2.0f}));
    }

    quadShader.setBool("checkTL", checkTL);
    quadShader.setBool("checkTR", checkTR);
    quadShader.setBool("checkBL", checkBL);
    quadShader.setBool("checkBR", checkBR);

    quadShader.setVec2("borderTL", borderTL);
    quadShader.setVec2("borderTR", borderTR);
    quadShader.setVec2("borderBL", borderBL);
    quadShader.setVec2("borderBR", borderBR);
}

void Quad::onWindowResize(const glm::vec2 &windowSize) {
    _lastWindowSize = windowSize;
    calculateModelMatrix();
}

void Quad::calculateModelMatrix() {
    // Get translation based on anchor point
    auto _scaledPos = _pos.toPixels(_lastWindowSize);
    auto _scaledSize = _size.toPixels(_lastWindowSize) * 0.5f;
    auto _correctedPos = _scaledPos - _scaledSize * (_anchorPoint * 2.0f - 1.0f);

    // Set model matrix
    _modelMatrix = glm::mat4{1.0f};
    _modelMatrix = glm::translate(_modelMatrix, glm::vec3(_correctedPos, 0.0f));
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(_scaledSize, 1.0f));
    _modelMatrix = glm::rotate(_modelMatrix, _rotation, glm::vec3{0.0f, 0.0f, 1.0f});
}