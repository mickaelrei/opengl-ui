#version 330 core

// Vertex position, coming from vertex buffer
layout (location = 0) in vec3 p;

// Transform matrices
uniform mat4 model;
uniform mat4 projection;

// Point to send for fragment shader
out vec2 fragPos;

void main() {
	// Transformed point
	vec4 vert = projection * model * vec4(p, 1.0f);

	// Update vertex position
	gl_Position = vert;

	// Send 2d point to fragment shader
	fragPos = p.xy;
}
