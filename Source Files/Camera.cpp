#include "Camera.h"

Camera::Camera(const glm::vec3 position,const glm::vec3 up)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(40.0f), MouseSensitivity(0.1f), Zoom(45.0f), startPosition(position) {
    Position = position;
    WorldUp = up;
    Up = WorldUp;
    Yaw = -90.0f;
    Pitch = 0.0f;
    Right = glm::normalize(glm::cross(glm::normalize(Front), WorldUp));
    viewMatrix = glm::lookAt(Position, Position + Front, Up);
    
    updateCameraVectors();
}

void Camera::Start() {
    LookAt(lockPos.value());
}

glm::mat4 Camera::GetViewMatrix() const{
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
}

glm::vec3 Camera::GetPosition() const {
    return Position;
}

void Camera::LookAt(glm::vec3 lookAtPos) {
    Position = startPosition;
    Front = glm::normalize(lookAtPos - Position);  // Direction from camera to target
    Right = glm::normalize(glm::cross(Up, Front)); // Right direction
    Up = glm::normalize(glm::cross(Front, Right)); // Recompute Up
    viewMatrix = glm::lookAt(Position, lookAtPos, Up);  // Position, target position, Up
    lockPos = lookAtPos;
    Yaw = glm::degrees(glm::atan(Front.z / Front.x));
    float magnitudeXZ = sqrt(Front.x * Front.x + Front.z * Front.z);
    Pitch = glm::degrees(glm::atan(-Front.y/magnitudeXZ));
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void Camera::ProcessKeyboardInputs(GLFWwindow* window) {
    float currentFrame = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += MovementSpeed * Front * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position -= MovementSpeed * Front * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position -= glm::normalize(glm::cross(Front, Up)) * deltaTime * MovementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += glm::normalize(glm::cross(Front, Up)) * deltaTime * MovementSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && lockPos.has_value() == true) {
        LookAt(lockPos.value());

    }
}

void Camera::ProcessMouseInputs(GLFWwindow* window) {

    if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (firstMouse == true) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        else {
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
            ProcessMouseMovement(xoffset, yoffset);
            lastX = xpos;
            lastY = ypos;
        }
    }

}

void Camera::updateCameraVectors() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    dir.y = sin(glm::radians(Pitch));
    dir.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(dir);
    Right = glm::normalize(glm::cross(glm::normalize(Front), WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
    viewMatrix = glm::lookAt(Position, Position + Front, Up);
}
