#include <iostream>
#include <GL/glew.h>
#include <GLFW//glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
// VAO � o ponteiro do programa que estou rodando, espa�o de mem�ria que representa aquele objeto
// VBO � o ponteiro para os meus dados brutos, s�o os dados do objeto a ser representado pelo VAO 
// shaderProgram � qual programa estou rodando 
// todo programa pode ser chamado de shader
GLuint VAO, VBO, shaderProgram;

float toRadians = 3.1415f / 180.0f;

bool direction = false, directionSize = false;
float triOffset = 0.0f, triOffsetMax = 0.7f, triOffsetMin = -0.7f, triIncrement = 0.03f;
float triOffsetSize = 0.2f, triOffsetSizeMax = 1.2f, triOffsetSizeMin = 0.2f, triOffsetSizeIncrement = 0.03f;
float triCurrentAngle = 0.0f, triAngleIncrement = 6.0f;

// aqui estamos fazendo um programa (shader) em GLSL

// shader para renderizar pontos na tela
static const char* vertexShader = "                                                        \n\
#version 330                                                                               \n\
layout(location=0) in vec2 pos;                                                            \n\
uniform mat4 model;                                                                        \n\
                                                                                           \n\
void main() {                                                                              \n\
	gl_Position = model * vec4(pos.x, pos.y, 0.0, 1.0);                                    \n\
}                                                                                          \n\
";

static const char* fragmentShader = "                                                      \n\
#version 330                                                                               \n\
                                                                                           \n\
uniform vec3 triColor;																       \n\
out vec4 color;                                                                            \n\
                                                                                           \n\
void main() {                                                                              \n\
	color = vec4(triColor, 1.0);                                                           \n\
}                                                                                          \n\
";

void create_triangle() {
	GLfloat vertices[] = { // nosso buffer de vert�ces
		0.0f, 1.0f, // vertice 1 
		-1.0f, -1.0f, // vertice 2
		1.0f, -1.0f // vertice 3
	};

	// iniciar um VAO
	// vamos abrir um espa�o de mem�ria na placa de v�deo
	// pegamos o endere�o virutal da placa de video e guardamos ele na vari�vel l� em cima
	glGenVertexArrays(1, &VAO);

	// estamos dizendo que a partir desse momento todas as altera��es ser�o feitas nesse espa�o de mem�ria
	// agora tudo o que vamos criar ser� nesse espa�o de mem�ria como os pontos da nossa tela
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO); // alocar um buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // a partir desse momento estamos modificando esse buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // explicando onde est�o os dados
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // explicando como interpretar esses dados
	glEnableVertexAttribArray(0);  // location

	// deixar de apontar para o VAO e VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// fun��o para executar um buffer na tela
void add_shader(GLuint program, const char* shaderCode, GLenum type) {
	GLuint _shader = glCreateShader(type);

	// converte char para glchar
	const GLchar* code[1];
	code[0] = shaderCode;
	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);


	// tratar os erros

	glAttachShader(program, _shader);
}

void add_program() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		cout << "Erro: falha ao criar o programa!";
	}

	add_shader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
	add_shader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
}





int main() {
	// inicando GLFW
	if (!glfwInit()) {
		cout << "Erro: GLFW n�o foi iniciado";
		glfwTerminate();
		return -1;
	}

	// set nas coisas obrigat�rias para subir uma janela
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // vers�o m�xima permitida 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // vers�o m�nima
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // somente as fun��es core
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // qual a precis�o de ponto flutuante que vamos usar, precis�o da placa

	// cria��o de janela 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
	if (!window) {
		cout << "Erro: N�o foi poss�vel criar a janela";
		glfwTerminate();
		return -1;
	}

	// guardando o tamanho real da minha tela 
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(window); // tornando essa janela como principal

	//iniciando o Glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Erro: n�o foi poss�vel iniciar o glew";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	create_triangle();
	add_program();

	while (!glfwWindowShouldClose(window)) {
		//Cor de fundo da janela
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		//Altera cor do triangulo
		GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);

		//Movimenta o triangulo
		if (!direction)
			triOffset += triIncrement;
		else
			triOffset -= triIncrement;
		if (triOffset > triOffsetMax || triOffset < triOffsetMin)
			direction = !direction;

		triCurrentAngle += triAngleIncrement;
		if (triCurrentAngle >= 360)
			triCurrentAngle = 0;

		if (!directionSize)
			triOffsetSize += triOffsetSizeIncrement;
		else
			triOffsetSize -= triOffsetSizeIncrement;
		if (triOffsetSize > triOffsetSizeMax || triOffsetSize < triOffsetSizeMin)
			directionSize = !directionSize;

		GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(triOffsetSize, triOffsetSize, 0.0f));
		model = glm::rotate(model, triCurrentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


		//Desenhando o triangulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); //Tringulo, come�ando na posi��o 0, Numero de pontos 3
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}