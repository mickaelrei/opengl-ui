#version 330 core

#define ALPHA_OFFSET 0.005f

out vec4 fragColor;

// Quad data
uniform vec2 pos;
uniform vec2 size;
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
	vec2 corrected = fragPos * 0.5f + 0.5f;

	// Variables for corner check
	bool isCorner = false;
	float cx;
	float cy;
	float invRadiusX2;
	float invRadiusY2;
	float r;

	// Bottom left
	if (checkBL && corrected.x < borderBL.x && corrected.y < borderBL.y) {
		isCorner = true;
		invRadiusX2 = inv2BL.x;
		invRadiusY2 = inv2BL.y;
		cx = borderBL.x - corrected.x;
		cy = borderBL.y - corrected.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (-corrected.y - (corrected.x - 1) >= 0) r = test;
	}

	// Bottom right
	if (checkBR && corrected.x > (1.0f - borderBR.x) && corrected.y < borderBR.y) {
		isCorner = true;
		invRadiusX2 = inv2BR.x;
		invRadiusY2 = inv2BR.y;
		cx = borderBR.x + corrected.x - 1.0f;
		cy = borderBR.y - corrected.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (corrected.y - corrected.x <= 0) r = test;
	}

	// Top left
	if (checkTL && corrected.x < borderTL.x && corrected.y > (1.0f - borderTL.y)) {
		isCorner = true;
		invRadiusX2 = inv2TL.x;
		invRadiusY2 = inv2TL.y;
		cx = borderTL.x - corrected.x;
		cy = borderTL.y + corrected.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (corrected.y - corrected.x >= 0) r = test;
	}

	// Top right
	if (checkTR && corrected.x > (1.0f - borderTR.x) && corrected.y > (1.0f - borderTR.y)) {
		isCorner = true;
		invRadiusX2 = inv2TR.x;
		invRadiusY2 = inv2TR.y;
		cx = borderTR.x + corrected.x - 1.0f;
		cy = borderTR.y + corrected.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else if (-corrected.y - (corrected.x - 1) <= 0) r = test;
	}

	float alpha = 1.0f;
	float eps = 0.02f;
	if (isCorner) {
		alpha = smoothstep(1.0f, 1.0f - eps, r);
	}
	fragColor = vec4(corrected.x, corrected.y, 0.0f, 1.0f);
	// fragColor = vec4(alpha, 0.0f, 0.0f, 1.0f);
}
