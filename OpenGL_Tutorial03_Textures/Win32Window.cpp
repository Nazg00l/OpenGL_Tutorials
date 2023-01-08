// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include "Win32Window.h"

#include "Shader.h"

#include <iostream>

// -------------------->> NOTE <<----------------------
//
// Because m_ScreenWidth, m_ScreenHeight ,and m_Title are static members as declared in Win32Window.h
// they should be defined somewhere and without repeating the word static
// and also we initialized them btw

GLint Win32Window::m_ScreenWidth = 1;
GLint Win32Window::m_ScreenHeight = 1;
const char* Win32Window::m_Title = "";


GLFWwindow *Win32Window::CreateWindow(GLint width, GLint height, const char* title)
{
	if (width <= 0 || height <= 0)
		return nullptr;

	// 2) Initialize GLFW
	glfwInit();

	// 3) Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 4) Creat a GLFWWindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	// 5)
	/**
	* This is the actual or the accurate representation of what our window is
	* relative to our screen
	*/
	glfwGetFramebufferSize(window, &m_ScreenWidth, &m_ScreenHeight);
	m_Title = title;

	// 6) Test the window creation status
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	// 7) Make the context we created as the current context
	glfwMakeContextCurrent(window);

	return window;
}

void Win32Window::Run(GLFWwindow* window)
{

	// 8) Set this to true so GLEW knows to use a modern approach to retrieve function pointer and extentions
	glewExperimental = GL_TRUE;

	// 9) Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}

	// 10) Define the viewport  dimensions
	//glViewport(0, 0, ScreenWidth, ScreenHeight);
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

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
	// Create the Vertex array buffer object and Binding the vertex array object
	GLuint VAO;
	/*glCreateVertexArrays(1, &Vao);*/
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// Create the Vertex buffer object ,then binding and setting the vertex buffers
	GLuint VBO;
	/*glCreateBuffers(1, &VBO);*/
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enabling the vertex array

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));	// for the color
	glEnableVertexAttribArray(1); // enabling the vertex array
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
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
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
}


GLint Win32Window::GetScreenWidth()
{
	return m_ScreenWidth;
}
GLint Win32Window::GetScreenHeight()
{
	return m_ScreenHeight;
}