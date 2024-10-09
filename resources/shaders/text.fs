#version 330 core

out vec4 fragColor;

uniform sampler2D tex;

uniform vec4 color;

// Frag pos
in vec2 fragPos;

void main() {
	// Correct to [0, 1] range
	vec2 uv = fragPos;

	float texAlpha = texture(tex, uv).r;
	if (texAlpha < 0.1f) discard;

	float alpha = color.a * texAlpha;
	fragColor = vec4(color.rgb, alpha);
}
