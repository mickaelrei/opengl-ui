#include <glm/glm.hpp>

#include "glad/glad.h"

#include "debug.hpp"
#include "shader.hpp"
#include "text.hpp"

/// @brief Shader used to render text
static Shader textShader;

/// @brief OpenGL objects for text rendering
static unsigned int textVAO, textVBO, textEBO;

/// @brief Whether text resources are already initialized
static bool initialized = false;

namespace TextModule {

bool init(const std::string &rootPath, const glm::vec2 &windowSize) {
    if (initialized) return true;
    initialized = true;

    // Initialize shader and set initial uniforms
    // ------------------------------------------
    textShader = Shader{
        rootPath + "/resources/shaders/text.vs",
        rootPath + "/resources/shaders/text.fs"
    };
    onWindowResize(windowSize);

    // Construct VAO for text rendering
    // --------------------------------

    // Default vertices for quad
    const float __vertices[] = {
        // positions
        0.0f, 0.0f, // bottom left
        0.0f, 1.0f, // top left
        1.0f, 1.0f, // top right
        1.0f, 0.0f, // bottom right
    };

    // Default indices for quad
    const unsigned int __indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    // Create vertex objects
    glGenVertexArrays(1, &textVAO); glCheckError();

    glGenBuffers(1, &textVBO); glCheckError();
    glGenBuffers(1, &textEBO); glCheckError();

    // Bind the array (VAO) first
    glBindVertexArray(textVAO); glCheckError();

    // Then bind and set the buffer (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, textVBO); glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(__vertices), __vertices, GL_STATIC_DRAW); glCheckError();

    // Then bind and set the elements buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textEBO); glCheckError();
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
    debugPrint("Text module successfully loaded\n%s", "");
    return true;
}

void terminate() {
    if (!initialized) return;
    initialized = false;

    textShader.destroy();
    glDeleteBuffers(1, &textVBO); glCheckError();
    glDeleteBuffers(1, &textEBO); glCheckError();
    glDeleteVertexArrays(1, &textVAO); glCheckError();
}

void onWindowResize(const glm::vec2 &windowSize) {
    if (!initialized) return;

    textShader.setMat4("projection", glm::ortho(
        // left-right
        0.0f, windowSize.x,

        // top-bottom
        windowSize.y, 0.0f,

        // near-far
        0.0f, 1.0f
    ));
}

}

Text::Text(const std::string &text, const Font &font) : _text{text}, _font{font} {}

void Text::draw(const glm::vec2 &windowSize) {
    // Get projection
    auto projection = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, 0.0f, 1.0f);
    glm::mat4 model{1.0f};

    // Set base uniforms
    textShader.setMat4("projection", projection);
    textShader.setVec4("color", _color);
    textShader.setInt("tex", 0);

    // Base GL bindings
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO); glCheckError();

    // Calculate font scale based on given font size and font loaded height
    float scale = _fontSize / _font.fontHeight();

    // Keep track of current render position
    const float fontOffsetY = _font.maxCharHeight() - _font.maxCharUnderflow();
    float x = _topLeft.x;
    float y = _topLeft.y;

    bool startOfWord = true;
    const size_t textSize = _text.size();
    for (size_t i = 0; i < textSize; ++i) {
        char c = _text[i];
        auto charData = _font.getCharInfo(c);

        // Skip space char
        if (c == ' ') {
            x += charData.advance * scale;
            startOfWord = true;
            continue;
        }

        // Check if next word overflows width
        if (startOfWord && i != 0) {
            // Find next space
            size_t idx = _text.find(' ', i);

            // If not found, use end of string
            if (idx == -1UL) {
                idx = textSize;
            }
            auto word = _text.substr(i, idx - i);

            // Found next space, check if word is too large
            float textWidth = _font.calculateTextWidth(word, _fontSize);
            if (-_topLeft.x + x + textWidth > _renderWidth) {
                // Skip line and reset x
                x = _topLeft.x;
                y += _fontSize * _lineHeight;
            }
        } else if (i != 0 && -_topLeft.x + x + charData.advance * scale > _renderWidth) {
            // If not start of word, only skip line if current character overflows width
            x = _topLeft.x;
            y += _fontSize * _lineHeight;
        }

        // Reset word flag
        startOfWord = false;

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, charData.textureID);

        // Calculate offset
        float xpos = x + charData.bearing.x * scale;
        float ypos = y + (fontOffsetY - charData.bearing.y) * scale;

        // Calculate new model matrix
        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3{xpos, ypos, 0.0f});
        model = glm::scale(model, glm::vec3{charData.size * scale, 1.0f});

        // Change render position
        x += charData.advance * scale;

        textShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
    }

    // Unbind text VAO
    glBindVertexArray(0); glCheckError();
}

void Text::setText(const std::string &text) {
    _text = text;
}

std::string Text::text() const {
    return _text;
}

void Text::setFont(const Font &font) {
    _font = font;
}

Font Text::font() const {
    return _font;
}

void Text::setFontSize(float fontSize) {
    _fontSize = fontSize;
}

float Text::fontSize() const {
    return _fontSize;
}

void Text::setTopLeft(const glm::vec2 &topLeft) {
    _topLeft = topLeft;
}

glm::vec2 Text::topLeft() const {
    return _topLeft;
}

void Text::setRenderWidth(float renderWidth) {
    _renderWidth = renderWidth;
}

float Text::renderWidth() const {
    return _renderWidth;
}

void Text::setLineHeight(float lineHeight) {
    _lineHeight = lineHeight;
}

float Text::lineHeight() const {
    return _lineHeight;
}

void Text::setColor(const glm::vec4 &color) {
    _color = color;
}

glm::vec4 Text::color() const {
    return _color;
}

void Text::setAlignment(TextAlignment alignment) {
    _alignment = alignment;
}

TextAlignment Text::alignment() const {
    return _alignment;
}