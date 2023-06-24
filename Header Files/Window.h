#pragma once

#include <GLFW/glfw3.h>
#include <utility>

class Window {
public:
    GLFWwindow* m_Window;
    int m_Width, m_Height;

    Window(int width, int height, const char* title);
    ~Window();

    void Clear() const;
    void Update() const;
    bool Closed() const;
    bool ShouldClose() const;
    void SwapBuffers();
    void PollEvents();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        // adjust viewport and other operations here
        glViewport(0, 0, width, height);
    }

    std::pair<int, int> getSize();

private:
};
