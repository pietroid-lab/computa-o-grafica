#include <stdio.h>
#include <GL\glxew.h>
#include <GLFW\glfw3.h>


const GLint WIDTH = 800, HEIGHT = 900;

int main() {
	//Iniciat o GLFW
	if (!glfwInit()) {
		printf("glfw Não foi iniciado");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(1024, 1024, "janela 1", glfwGetPrimaryMonitor(), NULL);

	if (!window) {
		printf("Erro ao criar janela");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeigth;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeigth);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Erro ao iniciar o Glew!");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeigth);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f,1.0f,1.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
