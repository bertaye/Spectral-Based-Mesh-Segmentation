#pragma once
#include <memory>
#include"SVertex.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <Eigen/Sparse>
#include <GLM/glm/vec3.hpp>

class MeshRenderer; // Forward declaration

struct RawTriangle {
	int v1, v2, v3;
	RawTriangle(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3) {}
};

class Mesh {
public:

	Mesh() {};
	void loadOBJ(const char* filename);
	void loadOFF(const char* filename);
	void createLaplacianMatrix();
	std::optional<Eigen::VectorXf> calculateFiedlerVector();
	std::optional<glm::vec3> getBestCamPos();
	/// <summary>
	/// Returns the position of the mesh
	/// </summary>
	glm::vec3 getPos();
	unsigned int getVertexCount() const;

private:
	std::vector<RawVertex> rawVertices;
	std::vector<unsigned int> rawTriangles;

	glm::vec3 min = glm::vec3(FLT_MAX), max = glm::vec3(FLT_MIN);
	Eigen::SparseMatrix<float> laplacianMatrix;
	void AddAdjVertex(std::vector<int>& adjverts,int v1);
	friend class MeshRenderer;
};