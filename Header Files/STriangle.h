#pragma once
#include <glm/vec3.hpp>
#include "SVertex.h"
#include <array>

struct RawTriangle {
	int v1, v2, v3;
	RawTriangle(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3) {}
};