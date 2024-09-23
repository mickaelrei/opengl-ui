#version 330 core

out vec4 fragColor;

uniform sampler2D tex;

uniform vec2 offset;
uniform vec2 size;
uniform vec4 color;

// Frag pos
in vec2 fragPos;

void main() {
	// Correct to [0, 1] range
	vec2 uv = fragPos * 0.5f + 0.5f;

	// Apply offset and size
	uv = uv * size + offset;

	float texAlpha = texture(tex, uv).r;
	float alpha = color.a * texAlpha;
	fragColor = vec4(color.rgb, alpha);
	// fragColor = texture(tex, uv);
}
