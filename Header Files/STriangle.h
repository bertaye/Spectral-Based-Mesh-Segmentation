#pragma once
#include <GLM/glm/vec3.hpp>
#include "SVertex.h"
#include <array>

struct Vertex;  // Forward declaration

struct Triangle {
	std::array<std::shared_ptr<Vertex>, 3> vertices;
	int index;
	int midPointIdx = -1;
	glm::vec3 normals;
	Triangle(int id, std::array<std::shared_ptr<Vertex>, 3> verts) : index(id), vertices(verts) {
	};
};