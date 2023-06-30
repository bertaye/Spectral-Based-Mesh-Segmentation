#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Window.h"

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
    myMesh.loadOFF("C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/3DObjects/125.off");
    myMesh.createLaplacianMatrix();


    Camera camera(myMesh.getBestCamPos().value(), glm::vec3(0.0f, 1.0f, 0.0f));
    // Load a shader program
    Shader myShader;
    myShader.Generate("C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/Shaders/vertexShader.glsl", "C:/Cpp_Projects/Spectral_Based_Mesh_Segmentation/Shaders/fragmentShader.glsl");
    // Create a mesh renderer
    MeshRenderer meshRenderer(myMesh, myShader);
    myMesh.segment(5);
    meshRenderer.PaintVertices();
    meshRenderer.Render(window,camera);
    

    glfwTerminate();

    return 0;
}
