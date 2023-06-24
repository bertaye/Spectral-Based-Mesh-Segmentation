#pragma once
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <optional>
class Camera {
public:
    Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, -200.0f),const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    void Start();
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::vec3 GetPosition() const;
    void LookAt(glm::vec3 lookAtPos);
    void ProcessMouseScroll(float yoffset);
    void ProcessKeyboardInputs(GLFWwindow* window);
    void ProcessMouseInputs(GLFWwindow* window);
private:
    bool firstMouse = true;
    float lastX = 0.0f;
    float lastY = 0.0f;

    glm::vec3 Position;
    const glm::vec3 startPosition;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 rotation;
    std::optional<glm::vec3> lockPos;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    glm::mat4 viewMatrix;
    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    void updateCameraVectors();
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

};