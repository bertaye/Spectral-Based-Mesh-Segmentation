#include "MeshRenderer.h"
#include <GLEW/include/GL/glew.h>
#include <GLM/glm/gtc/type_ptr.hpp>
#include <iostream>

MeshRenderer::MeshRenderer(Mesh& mesh, Shader& shader):m_Mesh(mesh), m_Shader(shader) {
	setupMesh();
}

void MeshRenderer::Render(const Window& window, Camera& camera) {

	int bufferWidth, bufferHeight;

	glfwGetFramebufferSize(window.m_Window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window.m_Window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(window.m_Window);
		glfwTerminate();
		return;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	setOpenGLPreferences(window);
	getUniformLocations();
	m_Shader.Use();



	float lastX = window.m_Width / 2.0f, lastY = window.m_Height / 2.0f;
	bool firstMouse = false;

	camera.LookAt(m_Mesh.getPos());
	camera.Start();
	while (!window.ShouldClose()) {
		PreRender();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_Shader.SetMat4("transform", modelMatrix);
		m_Shader.SetMat4("model", modelMatrix);
		m_Shader.SetMat4("view", camera.GetViewMatrix());
		m_Shader.SetMat4("projection", camera.GetProjectionMatrix());
		lightPos = camera.GetPosition();
		m_Shader.SetVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		m_Shader.SetVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
		m_Shader.SetVec3("objectColor", objectColor.x, objectColor.y, objectColor.z);
		glfwPollEvents();

		camera.ProcessMouseInputs(window.m_Window);
		camera.ProcessKeyboardInputs(window.m_Window);

		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, m_Mesh.rawTriangles.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window.m_Window);
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			switch (err) {
			case GL_INVALID_ENUM:
				std::cout << "OpenGL error: " << "INVALID_ENUM" << std::endl;
			case GL_INVALID_VALUE:
				std::cout << "OpenGL error: " << "INVALID_VALUE" << std::endl;
			case GL_INVALID_OPERATION:
				std::cout << "OpenGL error: " << "INVALID_OPERATION" << std::endl;
			case GL_STACK_OVERFLOW:
				std::cout << "OpenGL error: " << "STACK_OVERFLOW" << std::endl;
			case GL_STACK_UNDERFLOW:
				std::cout << "OpenGL error: " << "STACK_UNDERFLOW" << std::endl;
			case GL_OUT_OF_MEMORY:
				std::cout << "OpenGL error: " << "OUT_OF_MEMORY" << std::endl;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cout << "OpenGL error: " << "INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			default:
				std::cout << "OpenGL error: " << "UNKNOWN_ERROR" << std::endl;
			}
		}

		PostRender();

	}

	dispose();

	// Render the mesh
}

void MeshRenderer::PaintVertices(const std::vector<glm::vec3>& colors) {
	for (int idx = 0; idx < colors.size() && idx < m_Mesh.rawVertices.size(); idx++) {
		m_Mesh.rawVertices[idx].color = colors[idx];
	}

	updateVertexColors(colors);
}

void MeshRenderer::setShaders(const char* vertexPath, const char* fragmentPath) {
}

void MeshRenderer::setPosition(const glm::vec3& position) {
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
}

void MeshRenderer::dispose() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
void MeshRenderer::PreRender() {
}

void MeshRenderer::PostRender() {
}

void MeshRenderer::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Mesh.rawVertices.size() * sizeof(RawVertex), &m_Mesh.rawVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Mesh.rawTriangles.size() * sizeof(unsigned int), &m_Mesh.rawTriangles[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertex), (void*)offsetof(RawVertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertex), (void*)offsetof(RawVertex, normals));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(RawVertex), (void*)offsetof(RawVertex, color));

	glBindVertexArray(0);
}

void MeshRenderer::updateVertexColors(const std::vector<glm::vec3>& colors) {

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Mesh.rawVertices.size() * sizeof(RawVertex), m_Mesh.rawVertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		switch (err) {
		case GL_INVALID_ENUM:
			std::cout << "OpenGL error: " << "INVALID_ENUM" << std::endl;
		case GL_INVALID_VALUE:
			std::cout << "OpenGL error: " << "INVALID_VALUE" << std::endl;
		case GL_INVALID_OPERATION:
			std::cout << "OpenGL error: " << "INVALID_OPERATION" << std::endl;
		case GL_STACK_OVERFLOW:
			std::cout << "OpenGL error: " << "STACK_OVERFLOW" << std::endl;
		case GL_STACK_UNDERFLOW:
			std::cout << "OpenGL error: " << "STACK_UNDERFLOW" << std::endl;
		case GL_OUT_OF_MEMORY:
			std::cout << "OpenGL error: " << "OUT_OF_MEMORY" << std::endl;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "OpenGL error: " << "INVALID_FRAMEBUFFER_OPERATION" << std::endl;
		default:
			std::cout << "OpenGL error: " << "UNKNOWN_ERROR" << std::endl;
		}
	}
}

void MeshRenderer::setOpenGLPreferences(const Window& window) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glfwSetInputMode(window.m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MeshRenderer::getUniformLocations() {
	transfLoc = glGetUniformLocation(m_Shader.ID, "transform");
	modelLoc = glGetUniformLocation(m_Shader.ID, "model");
	viewLoc = glGetUniformLocation(m_Shader.ID, "view");
	projectionLoc = glGetUniformLocation(m_Shader.ID, "projection");
	lightPosLoc = glGetUniformLocation(m_Shader.ID, "lightPos");
	lightColorLoc = glGetUniformLocation(m_Shader.ID, "lightColor");
	objectColorLoc = glGetUniformLocation(m_Shader.ID, "objectColor");
}
