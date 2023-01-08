
// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include "Shader.h"

#include <iostream>

// 1) Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[])
{

	// 2) Init GLFW
	if (!glfwInit())
	{
		std::cout << "glfwInit() failed\n";
		return -1;
	}

	// 3) Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 4) Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Tutorial 2 Shader Files", nullptr, nullptr);
	// 5) 
	/**
	* This is the actual or the accurate representation of what our window is
	* relative to our screen
	*/
	GLint ScreenWidth, ScreenHeight;
	glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);

	// 6) Test the window creation status
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// 7) Make the context we created as the current context
	glfwMakeContextCurrent(window);

	// 8) Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// 9) Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// 10) Define the viewport dimensions 
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// These is an addition to show the desired info below:
	std::cout << "OpenGL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "GLSL   version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl << std::endl;

	/**
	* Define our vertex data that we are going to draw it
	*/
	// Create a Shader object
	Shader ourShader("core.vs", "core.frag");

	GLfloat vertices[] = {
		//Position				// Color
		0.0f, 0.5f, 0.0f,		 1.0f, 0.0f, 0.0f,		// Top middle vertex
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		// Bottom right vertex
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f		// Bottom left vertex
	};

	// Create(Generate) the Vertext Array object and the Vertex Buffer Object
	GLuint VAO;
	/*glCreateVertexArrays(1, &Vao);*/
	glGenVertexArrays(1, &VAO);
	// Bind the VAO so the next vbo will be bound to it automatically
	glBindVertexArray(VAO);
	// Generate the Vertex buffer object ,then bind an area of storage for our VBO
	GLuint VBO;
	/*glCreateBuffers(1, &VBO);*/
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Define the data for our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Tell OpenGL How to Link the VBO data with the a specific attribute in the Vertex Shader. 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*) 0);
	glEnableVertexAttribArray(0); // enabling the vertex attribute array (The attribute)

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));	// for the color
	glEnableVertexAttribArray(1); // enabling the vertex attribute array (The attribute)
	// unbinding
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 means unbinding any buffer object previously bound.
	glBindVertexArray(0); // 0 means break (unbinding) the existing vertex array object binding.
	

	// 11) Now create our game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 1.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw opengl stuff
		ourShader.Use();
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // 0 means break (unbinding) the existing vertex array object binding.

		// Swap the screen buffers
		glfwSwapBuffers(window);

		/**
		* This will handle the Escape key to easily return to the desktop
		* helpful If you are making a fullscreen application
		*/
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// deallocating the resources that are out of use
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// 12) Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS;

}