#include "Mesh.h"
#include <queue>
#include <memory>
#include <Eigen/Eigenvalues>
#include <GLM/glm/glm.hpp>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/SymEigsShiftSolver.h>
#include <Spectra/MatOp/SparseSymShiftSolve.h>
#include <chrono>
#include <iostream>


void Mesh::loadOBJ(const char* filename) {

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    glm::vec3 normals(0.0F);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            // Load vertex
            glm::vec3 coords;
            ss >> coords.x >> coords.y >> coords.z;
            int idx = rawVertices.size();
            rawVertices.push_back(RawVertex(idx, coords.x, coords.y, coords.z));
            rawVertices[idx].normals = normals;
            min = glm::min(min, rawVertices[idx].position);
            max = glm::max(max, rawVertices[idx].position);
        }
        else if (type == "vn"){
            ss >> normals.x >> normals.y >> normals.z;
        }
        else if (type == "f") {
            // Store face definition for later
            int v1, v2, v3;
            std::string v1_str, v2_str, v3_str;

            ss >> v1_str >> v2_str >> v3_str;
            v1 = std::stoi(v1_str.substr(0, v1_str.find("//")));
            v2 = std::stoi(v2_str.substr(0, v2_str.find("//")));
            v3 = std::stoi(v3_str.substr(0, v3_str.find("//")));

            rawTriangles.push_back(v1-1);
            rawTriangles.push_back(v2-1);
            rawTriangles.push_back(v3-1);
            AddAdjVertex(rawVertices[v1 - 1].adjVerts, v2 - 1);
            AddAdjVertex(rawVertices[v1 - 1].adjVerts, v3 - 1);

            AddAdjVertex(rawVertices[v2 - 1].adjVerts, v1 - 1);
            AddAdjVertex(rawVertices[v2 - 1].adjVerts, v3 - 1);

            AddAdjVertex(rawVertices[v3 - 1].adjVerts, v1 - 1);
            AddAdjVertex(rawVertices[v3 - 1].adjVerts, v2 - 1);


        }
    }

    file.close();
}

unsigned int Mesh::getVertexCount() const {
    return rawVertices.size();
}

void Mesh::loadOFF(const char* filename) {
    int nVerts, nTris, n, i=0;
    int v1 = 0, v2 = 0, v3 = 0;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    if(line == "OFF")
        std::getline(file, line);

    std::stringstream ss(line);
    ss >> nVerts >> nTris >> n;
    while (i++ < nVerts) {
        std::getline(file, line);
        ss = std::stringstream(line);
        glm::vec3 coords;
        ss >> coords.x >> coords.y >> coords.z;
        int idx = rawVertices.size();
        rawVertices.push_back(RawVertex(idx, coords.x, coords.y, coords.z));
        min = glm::min(min, rawVertices[idx].position);
        max = glm::max(max, rawVertices[idx].position);
    }

    while (file.peek() != EOF ) {
        char discard;
        std::getline(file, line);
        ss = std::stringstream(line);
        ss >> discard >> v1 >> v2 >> v3;
        rawTriangles.push_back(v1);
        rawTriangles.push_back(v2);
        rawTriangles.push_back(v3);
        glm::vec3 normal = glm::normalize(glm::cross(rawVertices[v2].position - rawVertices[v1].position, rawVertices[v3].position - rawVertices[v1].position));
        rawVertices[v1].normals = normal;
        rawVertices[v2].normals = normal;
        rawVertices[v3].normals = normal;

        AddAdjVertex(rawVertices[v1].adjVerts, v2);
        AddAdjVertex(rawVertices[v1].adjVerts, v3);

        AddAdjVertex(rawVertices[v2].adjVerts, v1);
        AddAdjVertex(rawVertices[v2].adjVerts, v3);
        
        AddAdjVertex(rawVertices[v3].adjVerts, v1);
        AddAdjVertex(rawVertices[v3].adjVerts, v2);
    }
}

std::optional<glm::vec3> Mesh::getBestCamPos() {
    glm::vec3 center = (min + max) / 2.0f;
    float maxDimension = glm::distance(min, max);
    glm::vec3 cameraPos = center + glm::vec3(0.0f, 0.0f, maxDimension);
    return cameraPos;
}

glm::vec3 Mesh::getPos() {
    return (min + max) / 2.0f;
}

void Mesh::AddAdjVertex(std::vector<int>& adjVerts, int v1) {
    if (std::count(adjVerts.begin(), adjVerts.end(), v1) == 0) {
        adjVerts.push_back(v1);
    }
}

std::optional<Eigen::VectorXf> Mesh::calculateFiedlerVector() {

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Spectra::SparseSymMatProd<float> op(laplacianMatrix);

    // Construct eigen solver object, requesting the largest three eigenvalues
    Spectra::SymEigsSolver<Spectra::SparseSymMatProd<float>> eigs(op, 2, 6);
    eigs.init();

    int nconv = eigs.compute(Spectra::SortRule::SmallestAlge,4000,1.0E-7);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Passed time for Fiedler Vect calculation: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
    // Retrieve results
    if (eigs.info() == Spectra::CompInfo::Successful) {
        Eigen::VectorXf eigenvalues = eigs.eigenvalues();
        Eigen::MatrixXf eigenvectors = eigs.eigenvectors();
        std::cout << "Eigenvalues: " << eigenvalues << std::endl;
        
        return static_cast<Eigen::VectorXf>(eigenvectors.col(0));;
    }
    std::cout << "Fiedler vector calculation failed." << std::endl;
}

void Mesh::createLaplacianMatrix() {
    if (rawVertices.size() == 0) {
        std::cerr << "Laplacian matrix cannot be created" << std::endl;
        return;
    }
    laplacianMatrix = Eigen::SparseMatrix<float>(rawVertices.size(), rawVertices.size());
    laplacianMatrix.setZero();

    std::vector<bool> visited(rawVertices.size(), false);

    for (auto& vertex : rawVertices) {
        visited[vertex.index] = true;
        laplacianMatrix.coeffRef(vertex.index, vertex.index) = vertex.adjVerts.size(); //degree of vertex
        for (auto& adjVert : vertex.adjVerts) {
            laplacianMatrix.coeffRef(vertex.index, adjVert) = -1; //adjacency, D-A
        }
    }
}

