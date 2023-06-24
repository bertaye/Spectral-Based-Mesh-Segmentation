#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title) {
    m_Width = width;
    m_Height = height;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_FOCUSED, false);

    m_Window = glfwCreateWindow(m_Width, m_Height, title, nullptr, nullptr);

    if (!m_Window) {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, m_Width, m_Height);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

}

Window::~Window() {
    glfwTerminate();
}

void Window::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update() const {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::Closed() const {
    return glfwWindowShouldClose(m_Window) == 1;
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

std::pair<int, int> Window::getSize() {
    glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
    return { m_Width, m_Height };
}
