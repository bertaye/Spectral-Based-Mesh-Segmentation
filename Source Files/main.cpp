#define _USE_MATH_DEFINES

#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Window.h"

void gaussianFilter(std::vector<float>& data, int N, float argSigma) {
    std::vector<float> result(data.size());
    double sigma = argSigma;
    std::vector<float> filter(N);
    float sum = 0;

    for (int i = 0; i < N; ++i) {
        int x = i - N / 2;
        filter[i] = std::exp(-(x * x) / (2 * sigma * sigma)) / std::sqrt(2 * M_PI * sigma * sigma);
        sum += filter[i];
    }

    for (int i = 0; i < N; ++i) {
        filter[i] /= sum;
    }

    for (int i = 0; i < data.size(); ++i) {
        float weightedSum = 0;
        for (int j = 0; j < N; ++j) {
            int index = i + j - N / 2;
            if (index >= 0 && index < data.size()) {
                weightedSum += data[index] * filter[j];
            }
        }
        result[i] = weightedSum;
    }

    data = result;
}


int main() {
    // Create a window
    Window window(1280, 1024, "My Window");


    if (glewInit() != GLEW_OK) {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window.m_Window);
        glfwTerminate();
        return -1;
    }
    // Create a camera

    // Load a mesh from an OBJ file
    Mesh myMesh;
    myMesh.loadOFF("3DObjects/125.off");
    myMesh.createLaplacianMatrix();
    std::optional<Eigen::VectorXf> fiedlerVect;// = myMesh.calculateFiedlerVector();


    Camera camera(myMesh.getBestCamPos().value(), glm::vec3(0.0f, 1.0f, 0.0f));
    // Load a shader program
    Shader myShader;
    myShader.Generate("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
    // Create a mesh renderer
    MeshRenderer meshRenderer(myMesh, myShader);
    if (fiedlerVect.has_value()) {
        int desiredSegmentCount = 6;
        int K = 6; //segment count
        int n = fiedlerVect.value().size();
        std::vector<glm::vec3> colors(n);

        std::vector<float> secondOrderDifferences;
        std::vector<int> localMaximaIndices;
        std::vector<float> currentHighests(K);
        std::vector<std::pair<int, float>> fiedlerVectorized;
        for (int i = 0; i < fiedlerVect.value().size(); i++) {
            fiedlerVectorized.push_back(std::make_pair(i, fiedlerVect.value()(i)));

        }

        std::sort(fiedlerVectorized.begin(), fiedlerVectorized.end(),
            [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second < b.second;
            });

        //secondOrderDifferences.push_back(-1);
        for (int i = 1; i < n - 1; i++) {
            secondOrderDifferences.push_back(std::abs(fiedlerVectorized[i - 1].second - 2 * fiedlerVectorized[i].second + fiedlerVectorized[i + 1].second));
        }

        gaussianFilter(secondOrderDifferences,secondOrderDifferences.size(), secondOrderDifferences.size()/70.0f); //was 40

        for (int i = 1; i < n - 3; i++) {
            if (secondOrderDifferences[i-1] < secondOrderDifferences[i] && secondOrderDifferences[i] > secondOrderDifferences[i+1]) {
                localMaximaIndices.push_back(i);
            }
        }
        while (localMaximaIndices.size() != desiredSegmentCount && localMaximaIndices.size()>1) {

            if (localMaximaIndices.size() > desiredSegmentCount) {
                localMaximaIndices.clear();
                gaussianFilter(secondOrderDifferences, secondOrderDifferences.size(), secondOrderDifferences.size() / 50.0f); //was 40

                for (int i = 1; i < n - 3; i++) {
                    if (secondOrderDifferences[i - 1] < secondOrderDifferences[i] && secondOrderDifferences[i] > secondOrderDifferences[i + 1]) {
                        localMaximaIndices.push_back(i);
                    }
                }
            }
            else {
                std::cout << "Desired segmentation number can not be reached." << std::endl;
                break;
            }
        }
        
        std::sort(localMaximaIndices.begin(), localMaximaIndices.end());
        int maxElIdx = std::max_element( secondOrderDifferences.begin(), secondOrderDifferences.end())- secondOrderDifferences.begin();

        std::vector<glm::vec3> paintColors;
        paintColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        paintColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        paintColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        paintColors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
        paintColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
        paintColors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        paintColors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
        paintColors.push_back(glm::vec3(0.25f, 1.0f, 0.25f));

        std::cout << "Found segmentable regions: "<<localMaximaIndices.size() << std::endl;

        int fiedlerSize = fiedlerVectorized.size();
        int colorIdx = 0;
        for (int i = 0; i < fiedlerSize; i++) {
            if (colorIdx < localMaximaIndices.size() && i > localMaximaIndices[colorIdx] && colorIdx < (paintColors.size() - 1) ) {
                colorIdx++;
            }
            colors[fiedlerVectorized[i].first] = paintColors[colorIdx];
        }
        meshRenderer.PaintVertices(colors);
    }
    meshRenderer.Render(window,camera);
    

    glfwTerminate();

    return 0;
}
