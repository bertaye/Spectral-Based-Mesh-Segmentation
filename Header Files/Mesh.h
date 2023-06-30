#pragma once
#include <memory>
#include "SVertex.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <map>
#include <Sparse>
#include <glm/vec3.hpp>

class MeshRenderer; // Forward declaration

class Mesh {
public:

	Mesh() {};
	void loadOBJ(const char* filename);
	void loadOFF(const char* filename);
	void createLaplacianMatrix();
	std::optional<Eigen::VectorXf> calculateFiedlerVector();
	/// <summary>
	/// K is the number of segmentations
	/// </summary>
	/// <param name="K"></param>
	void segment(int K);
	std::optional<glm::vec3> getBestCamPos();
	/// <summary>
	/// Returns the position of the mesh
	/// </summary>
	glm::vec3 getPos();
	unsigned int getVertexCount() const;

private:
	std::vector<RawVertex> rawVertices;
	std::vector<unsigned int> rawTriangles;
	void gaussianFilter(std::vector<float>& data, int N, float argSigma);
	glm::vec3 min = glm::vec3(FLT_MAX), max = glm::vec3(FLT_MIN);
	Eigen::SparseMatrix<float> laplacianMatrix;
	std::optional<Eigen::VectorXf> fiedlerVector;
	void AddAdjVertex(std::vector<int>& adjverts,int v1);
	friend class MeshRenderer;
};