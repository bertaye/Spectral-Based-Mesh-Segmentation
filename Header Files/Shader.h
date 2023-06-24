#pragma once

#include <GL/glew.h>
#include <string>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/glm.hpp>
class Shader {
public:
    // Program ID
    GLuint ID=0;

    // Constructor
    Shader() {}

    // Generates the shader on the fly
    void Generate(const char* vertexPath, const char* fragmentPath);

    // Use/Activate the shader
    void Use();

    // Utility functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, float v1, float v2, float v3) const;
    void SetVec4(const std::string& name, float v1, float v2, float v3, float v4) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);
};
