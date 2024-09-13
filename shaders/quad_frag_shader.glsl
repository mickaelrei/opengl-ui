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
	vec2 corrected = fragPos / size - pos / size;
	corrected = corrected * 0.5f + 0.5f;

	// Sanity check border radiuses
	vec2 cborderTL = clamp(borderTL, vec2(0.0f), vec2(1.0f)) * 0.5f;
	vec2 cborderTR = clamp(borderTR, vec2(0.0f), vec2(1.0f)) * 0.5f;
	vec2 cborderBL = clamp(borderBL, vec2(0.0f), vec2(1.0f)) * 0.5f;
	vec2 cborderBR = clamp(borderBR, vec2(0.0f), vec2(1.0f)) * 0.5f;

	// cache inverses
    bool drawTL = false;
	bool drawTR = false;
	bool drawBL = false;
	bool drawBR = false;
	vec2 invTL2;
	vec2 invTR2;
	vec2 invBL2;
	vec2 invBR2;

    if (cborderTL.x*cborderTL.y > 0) {
        drawTL = true;
        invTL2 = 1.0f / pow(cborderTL, vec2(2.0f));
	}
    if (cborderTR.x*cborderTR.y > 0) {
        drawTR = true;
		invTR2 = 1.0f / pow(cborderTR, vec2(2.0f));
	}
    if (cborderBL.x*cborderBL.y > 0) {
        drawBL = true;
		invBL2 = 1.0f / pow(cborderBL, vec2(2.0f));
	}
    if (cborderBR.x*cborderBR.y > 0) {
        drawBR = true;
		invBR2 = 1.0f / pow(cborderBR, vec2(2.0f));
	}

	// Variables for corner check
	bool isCorner = false;
	float cx;
	float cy;
	float invRadiusX2;
	float invRadiusY2;

	// Bottom left
	if (drawBL && corrected.x < cborderBL.x && corrected.y < cborderBL.y) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderBL.x * cborderBL.x);
		invRadiusY2 = 1.0f / (cborderBL.y * cborderBL.y);
		cx = cborderBL.x - corrected.x;
		cy = cborderBL.y - corrected.y;
	}

	// Bottom right
	if (drawBR && corrected.x > (1.0f - cborderBR.x) && corrected.y < cborderBR.y) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderBR.x * cborderBR.x);
		invRadiusY2 = 1.0f / (cborderBR.y * cborderBR.y);
		cx = cborderBR.x + corrected.x - 1.0f;
		cy = cborderBR.y - corrected.y;
	}

	// Top left
	if (drawTL && corrected.x < cborderTL.x && corrected.y > (1.0f - cborderTL.y)) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderTL.x * cborderTL.x);
		invRadiusY2 = 1.0f / (cborderTL.y * cborderTL.y);
		cx = cborderTL.x - corrected.x;
		cy = cborderTL.y + corrected.y - 1.0f;
	}

	// Top right
	if (drawTR && corrected.x > (1.0f - cborderTR.x) && corrected.y > (1.0f - cborderTR.y)) {
		isCorner = true;
		invRadiusX2 = 1.0f / (cborderTR.x * cborderTR.x);
		invRadiusY2 = 1.0f / (cborderTR.y * cborderTR.y);
		cx = cborderTR.x + corrected.x - 1.0f;
		cy = cborderTR.y + corrected.y - 1.0f;
	}

	if (isCorner && (cx*cx*invRadiusX2 + cy*cy*invRadiusY2) > 1.0f) discard;

	fragColor = vec4(corrected.x, corrected.y, 0.0f, 1.0f);
}
