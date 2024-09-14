#version 330 core

#define ALPHA_OFFSET 0.005f

out vec4 fragColor;

// Quad data
uniform vec2 pos;
uniform vec2 size;
uniform vec4 color;

uniform vec2 borderTL;
uniform vec2 borderTR;
uniform vec2 borderBL;
uniform vec2 borderBR;

// Frag pos
in vec2 fragPos;

void main() {
	// Correct to [0, 1] range
	vec2 corrected = fragPos * 0.5f + 0.5f;

	// Sanity check border radiuses
	vec2 cborderTL = clamp(borderTL, vec2(0.0f), vec2(1.0f));
	vec2 cborderTR = clamp(borderTR, vec2(0.0f), vec2(1.0f));
	vec2 cborderBL = clamp(borderBL, vec2(0.0f), vec2(1.0f));
	vec2 cborderBR = clamp(borderBR, vec2(0.0f), vec2(1.0f));

	// Correct radius overlap
	{
		float total;
		float div;

		// Left Y
		total = cborderTL.y + cborderBL.y;
		if (total > 1.0f) {
			div = 1.0f / total;
			cborderTL *= div;
			cborderBL *= div;
		}

		// Right Y
		total = cborderTR.y + cborderBR.y;
		if (total > 1.0f) {
			div = 1.0f / total;
			cborderTR *= div;
			cborderBR *= div;
		}

		// Top X
		total = cborderTL.x + cborderTR.x;
		if (total > 1.0f) {
			div = 1.0f / total;
			cborderTL *= div;
			cborderTR *= div;
		}

		// Bottom X
		total = cborderBL.x + cborderBR.x;
		if (total > 1.0f) {
			div = 1.0f / total;
			cborderBL *= div;
			cborderBR *= div;
		}
	}

	// Cache inverses
    bool checkTL = false;
	bool checkTR = false;
	bool checkBL = false;
	bool checkBR = false;

    if (cborderTL.x * cborderTL.y > 0)
        checkTL = true;
    if (cborderTR.x * cborderTR.y > 0)
        checkTR = true;
    if (cborderBL.x * cborderBL.y > 0)
        checkBL = true;
    if (cborderBR.x * cborderBR.y > 0)
        checkBR = true;

	// Variables for corner check
	bool isCorner = false;
	float cx;
	float cy;
	float invRadiusX2;
	float invRadiusY2;
	float r;

	// Bottom left
	if (checkBL && corrected.x < cborderBL.x && corrected.y < cborderBL.y) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderBL.x * cborderBL.x);
		invRadiusY2 = 1.0f / (cborderBL.y * cborderBL.y);
		cx = cborderBL.x - corrected.x;
		cy = cborderBL.y - corrected.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else r = test;
	}

	// Bottom right
	if (checkBR && corrected.x > (1.0f - cborderBR.x) && corrected.y < cborderBR.y) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderBR.x * cborderBR.x);
		invRadiusY2 = 1.0f / (cborderBR.y * cborderBR.y);
		cx = cborderBR.x + corrected.x - 1.0f;
		cy = cborderBR.y - corrected.y;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else r = test;
	}

	// Top left
	if (checkTL && corrected.x < cborderTL.x && corrected.y > (1.0f - cborderTL.y)) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderTL.x * cborderTL.x);
		invRadiusY2 = 1.0f / (cborderTL.y * cborderTL.y);
		cx = cborderTL.x - corrected.x;
		cy = cborderTL.y + corrected.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else r = test;
	}

	// Top right
	if (checkTR && corrected.x > (1.0f - cborderTR.x) && corrected.y > (1.0f - cborderTR.y)) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderTR.x * cborderTR.x);
		invRadiusY2 = 1.0f / (cborderTR.y * cborderTR.y);
		cx = cborderTR.x + corrected.x - 1.0f;
		cy = cborderTR.y + corrected.y - 1.0f;

		float test = cx * cx * invRadiusX2 + cy * cy * invRadiusY2;
		if (test > 1.0f) discard;
		else r = test;
	}

	float alpha = 1.0f;
	// if (isCorner) {
	// 	float mSize = max(size.x, size.y);
	// 	alpha = smoothstep(1.0f, 1.0f - 0.03f, r - 0.0f);
	// }
	fragColor = vec4(corrected.x, corrected.y, 0.0f, alpha);
	// fragColor = vec4(alpha, 0.0f, 0.0f, 1.0f);
}
