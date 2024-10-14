#version 330 core

// Output color
out vec4 fragColor;

// Character texture
uniform sampler2D charTexture;

// Character color
uniform vec4 color;

// Frag pos
in vec2 fragPos;

void main() {
	float texAlpha = texture(charTexture, fragPos).r;
	if (texAlpha < 0.1f) discard;

	float alpha = color.a * texAlpha;
	fragColor = vec4(color.rgb, alpha);
}
