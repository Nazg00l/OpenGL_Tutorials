// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include "SOIL2\SOIL2.h"

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
	if (!glfwInit())
	{
		std::cout << "glfwInit() failed\n";
		return nullptr;
	}

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

	// Enable the Blending
	// Enabling alpha support for images: png, .....
	glEnable(GL_BLEND);
	// Pixels can be drawn using a function that blends the incoming (source) RGBA 
	// values with the RGBA values that are already in the frame buffer (the destination values).
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/**
	* Define our vertex data that we are going to draw it
	*/
	// Create a Shader object
	Shader ourShader("core.vs", "core.frag");
	// Set up vertex
	GLfloat vertices[] = {
		//Position				// Color				// Texture Coordinates
		0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		// Top right vertex
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		// Bottom right vertex
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// Bottom left vertex
		-0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f		// Top left vertex
	};
	GLfloat vertices2[] = {
		//Position				// Color				// Texture Coordinates
		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		// Top right vertex
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		// Bottom right vertex
		-1.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// Bottom left vertex
		-1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f		// Top left vertex
	};
	// Set up indecies
	GLuint indices[] = {
		//0, 1, 2,	// First triangle
		//1, 2, 3		// Second triangle
		0, 1, 3,
		1, 2, 3
	};

	// Create(Generate) the Vertext Array object and the Vertex Buffer Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// Bind the VAO so the next vbo will be bound to it automatically
	glBindVertexArray(VAO);
	// Generate the Vertex buffer object ,then bind an area of storage for our VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Define the data for our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Create the indices 
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//
	// Pointer to Attribute
	// Tell OpenGL How to Link the VBO data with the a specific attribute in the Vertex Shader. 
	//
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enabling the vertex attribute array (The attribute)
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));	// for the color
	glEnableVertexAttribArray(1); // enabling the vertex attribute array (The attribute)
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));	// for the texture
	glEnableVertexAttribArray(2);
	// unbinding
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // 0 means unbinding any buffer object previously bound.
	glBindVertexArray(0); // 0 means break (unbinding) the existing vertex array object binding.

	/**
	* Loading the textures using SOIL
	*/
	// Create the texture
	GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Setting the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Loading and creating the texture then generating the mipmaps
	unsigned char* image = SOIL_load_image("ovechki.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// 11) Now create our game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.25f, 0.51f, 0.51f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw opengl stuff
		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);


		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	glDeleteBuffers(1, &EBO);

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