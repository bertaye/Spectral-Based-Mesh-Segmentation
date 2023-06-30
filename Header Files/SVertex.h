#pragma once
#include <vector>
#include <optional>
#include <glm/vec3.hpp>
#include "STriangle.h"
/// <summary>
/// A Raw Vertex data
/// </summary>
struct RawVertex {
	int index;
	std::vector<int> adjVerts;
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec3 color;
	RawVertex(int index, float x, float y, float z) :index(index), position(glm::vec3(x, y, z)), normals(glm::vec3(0.0f)), color(glm::vec3(1.0f)) {}
};