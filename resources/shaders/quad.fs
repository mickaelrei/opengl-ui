#version 330 core

// Output color
out vec4 fragColor;

// Quad data
uniform vec4 color;

uniform bool checkTL;
uniform bool checkTR;
uniform bool checkBL;
uniform bool checkBR;

uniform vec2 borderTL;
uniform vec2 borderTR;
uniform vec2 borderBL;
uniform vec2 borderBR;

uniform vec2 inv2TL;
uniform vec2 inv2TR;
uniform vec2 inv2BL;
uniform vec2 inv2BR;

// Frag pos
in vec2 fragPos;

void main() {
	// Correct to [0, 1] range
	vec2 uv = fragPos * 0.5f + 0.5f;
	// Shader is rendered from bottom-left to top-right,
	// but engine renders from top-left to bottom-right,
	// so we need to flip the Y component
	uv.y = 1.0f - uv.y;

	// Variables for corner check
	bool isCorner = false;
	float cx;
	float cy;
	float invRadiusX2;
	float invRadiusY2;
	float r;

	// Bottom left
	if (checkBL && uv.x < borderBL.x && uv.y < borderBL.y) {
		isCorner = true;
		invRadiusX2 = inv2BL.x;
		invRadiusY2 = inv2BL.y;
		cx = borderBL.x - uv.x;
		cy = borderBL.y - uv.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (-uv.y - (uv.x - 1) >= 0) r = test;
	}

	// Bottom right
	if (checkBR && uv.x > (1.0f - borderBR.x) && uv.y < borderBR.y) {
		isCorner = true;
		invRadiusX2 = inv2BR.x;
		invRadiusY2 = inv2BR.y;
		cx = borderBR.x + uv.x - 1.0f;
		cy = borderBR.y - uv.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (uv.y - uv.x <= 0) r = test;
	}

	// Top left
	if (checkTL && uv.x < borderTL.x && uv.y > (1.0f - borderTL.y)) {
		isCorner = true;
		invRadiusX2 = inv2TL.x;
		invRadiusY2 = inv2TL.y;
		cx = borderTL.x - uv.x;
		cy = borderTL.y + uv.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (uv.y - uv.x >= 0) r = test;
	}

	// Top right
	if (checkTR && uv.x > (1.0f - borderTR.x) && uv.y > (1.0f - borderTR.y)) {
		isCorner = true;
		invRadiusX2 = inv2TR.x;
		invRadiusY2 = inv2TR.y;
		cx = borderTR.x + uv.x - 1.0f;
		cy = borderTR.y + uv.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (-uv.y - (uv.x - 1) <= 0) r = test;
	}

	// TODO: This is to calculate smooth alpha on edges for Anti-Aliasing (not working yet)
	// float alpha = 1.0f;
	// float eps = 0.02f;
	// if (isCorner) {
	// 	alpha = smoothstep(1.0f, 1.0f - eps, r);
	// }
	// fragColor = vec4(alpha, 0.0f, 0.0f, 1.0f);

	// fragColor = vec4(uv.x, uv.y, 0.0f, 1.0f);
	fragColor = color;
}
