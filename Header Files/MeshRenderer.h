#pragma once

#include <vector>
#include <GLM/glm/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"

class MeshRenderer {
public:

    MeshRenderer(Mesh& mesh, Shader& shader);
    void Render(const Window& window, Camera& camera);
    void PaintVertices(const std::vector<glm::vec3>& colors);
    void setShaders(const char* vertexPath, const char* fragmentPath);
    void setPosition(const glm::vec3& position);
    void dispose();

private:
    glm::mat4 modelMatrix = glm::mat4(1.0F);
    Mesh& m_Mesh;
    Shader& m_Shader;

    // render data
    unsigned int VAO, VBO, EBO;
    // initializes all the buffer objects/arrays
    void PreRender();
    void PostRender();
    void setupMesh();
    void updateVertexColors(const std::vector<glm::vec3>& colors);
    void setOpenGLPreferences(const Window& window);
    void getUniformLocations();

    //LIGHTING
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);

    GLuint transfLoc;
    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projectionLoc;

    GLuint lightPosLoc;
    GLuint lightColorLoc;
    GLuint objectColorLoc;
    //LIGHTING
};
