// OpenGL1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "SOIL.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <exception>


struct Vertex {
	GLfloat position[3];
	GLfloat color[3];

};


int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello, World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//make it so we're not limited to 60 frames per second
	glfwSwapInterval(0);

	//triangle stuff

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


	GLuint testVAO;
	glGenVertexArrays(1, &testVAO);

	GLuint testVBO;
	glGenVertexArrays(1, &testVBO);

	glBindBuffer(GL_ARRAY_BUFFER, testVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), NULL, GL_STATIC_DRAW);
	Vertex* v = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	//0

	v->position[0] = 0.5f;
	v->position[1] = 0.5f;
	v->position[2] = 0.0f;

	v->color[0] = 1.0f;
	v->color[1] = 0.0f;
	v->color[2] = 0.0f;

	++v;

	//1
	v->position[0] = 0.5f;
	v->position[1] = -0.5f;
	v->position[2] = 0.0f;

	v->color[0] = 0.0f;
	v->color[1] = 1.0f;
	v->color[2] = 0.0f;

	++v;

	//2 
	v->position[0] = -0.5f;
	v->position[1] = -0.5f;
	v->position[2] = 0.0f;

	v->color[0] = 0.0f;
	v->color[1] = 0.0f;
	v->color[2] = 1.0f;

	++v;

	//3
	v->position[0] = -0.5f;
	v->position[1] = 0.5f;
	v->position[2] = 0.0f;

	v->color[0] = 1.0f;
	v->color[1] = 1.0f;
	v->color[2] = 0.0f;

	//glUnmapBuffer(testVBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glUnmapBuffer(GL_ARRAY_BUFFER);

	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	GLuint EBO;
	glGenBuffers(1, &EBO);


	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#if 0
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	
	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Because we are binding indices, we use GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif

	const char* vertexShaderSource = "#version 330 core\n \
		layout (location = 0) in vec3 position;\
		layout (location = 1) in vec3 color; \
		out vec3 ourColor;\
		out vec2 TexCoord;\
		void main() \
		{ \
			gl_Position = vec4(position, 1.0); \
			ourColor = color;\
		}";

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	const char* fragmentShaderSource = "#version 330 core\n \
		in vec3 ourColor;\
		out vec4 color; \
		uniform sampler2D ourTexture;\
		void main() \
		{ \
			color =  vec4(ourColor, 1.0f); \
		}";

	

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	
	
#if 0
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

#endif

	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//glBindVertexArray(0);
			
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point end_time;

	long double sumTime = 0.0f;
	int frames = 0;
	glUseProgram(shaderProgram);
	while (!glfwWindowShouldClose(window)) {

		if (sumTime > 1.0e6f) {
			printf("FPS: %i\n", frames);
			sumTime = 0.0f;
			frames = 0;
		}
		end_time = std::chrono::high_resolution_clock::now();
		sumTime += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
		start_time = std::chrono::high_resolution_clock::now();
		++frames;
		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &testVAO);
	glDeleteBuffers(1, &testVBO);
	glDeleteBuffers(1, &EBO);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//SOIL_free_image_data(image);
	
	glfwTerminate();
    return 0;
}

