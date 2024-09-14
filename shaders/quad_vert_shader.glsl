#version 330 core

// Vertex position, coming from vertex buffer
layout (location = 0) in vec2 p;

// Transform matrices
uniform mat4 model;
uniform mat4 projection;

// Point to send for fragment shader
out vec2 fragPos;

void main() {
	// Transformed point
	vec4 vert = projection * model * vec4(p, 0.0f, 1.0f);

	// Update vertex position
	gl_Position = vert;

	// Send unchanged position to fragment shader
	fragPos = p;
}
