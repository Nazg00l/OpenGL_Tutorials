// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include <SOIL2.h>

// The math library
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

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
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

	// These is an addition to show the desired info below:
	std::cout << "OpenGL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "GLSL   version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl << std::endl;

	/** Enable the Blending and the Depth */
	// Enabling alpha support for images: png, .....
	glEnable(GL_BLEND | GL_DEPTH_TEST);
	// Enabling depth for 3d objects
	glEnable(GL_DEPTH_TEST);
	// Pixels can be drawn using a function that blends the incoming (source) RGBA 
	// values with the RGBA values that are already in the frame buffer (the destination values).
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/**
	* Define our vertex data that we are going to draw it
	*/
	// Create a Shader object
	Shader ourShader("core.vs", "core.frag");
	// Set up vertex

	/*// This vertices are to be used with Perspective projection
	GLfloat vertices[] = {
		//Position				//Texture Coordinates
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// Top front right
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// Bottom front right
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// Bottom front left
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Top front left
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Top back right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// Bottom back right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Bottom back left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f		// Top back left
	};*/
	// This vertices are to be used with Perspective projection
	// by mezzo
	/*
	GLfloat vertices[] = {
		// Front face
		//Position				//Texture Coordinates
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// Right triangle Top right
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// Right triangle Bottom right
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// Right triangle Bottom left
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// Left triangle Bottom left
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Left triangle Top left
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// Left triangle Top Right
		// Right face
		//Position				//Texture Coordinates
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Right triangle Top right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// Right triangle Bottom right
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// Right triangle Bottom left
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// Left triangle Bottom left
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// Left triangle Top left
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Left triangle Top Right
		// Back face
		//Position				//Texture Coordinates
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// Right triangle Top right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Right triangle Bottom right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// Right triangle Bottom left
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// Left triangle Bottom left
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Left triangle Top left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// Left triangle Top Right
		// Left face
		//Position				//Texture Coordinates
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Right triangle Top right
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// Right triangle Bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Right triangle Bottom left
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Left triangle Bottom left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// Left triangle Top left
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Left triangle Top Right
		// Top face
		//Position				//Texture Coordinates
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Right triangle Top right
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// Right triangle Bottom right
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Right triangle Bottom left
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// Left triangle Bottom left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// Left triangle Top left
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// Left triangle Top Right
		// Bottom face
		//Position				//Texture Coordinates
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// Right triangle Top right
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// Right triangle Bottom right
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Right triangle Bottom left
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// Left triangle Bottom left
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// Left triangle Top left
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f		// Left triangle Top Right
	};*/
	
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	//
	// Pointer to Attribute
	// Tell OpenGL How to Link the VBO data with the a specific attribute in the Vertex Shader. 
	//
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enabling the vertex attribute array (The attribute)
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));	// for the texture
	glEnableVertexAttribArray(2); // enabling the vertex attribute array (The attribute)
	// unbinding
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
	//unsigned char* image = SOIL_load_image("ovechki.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	unsigned char* image = SOIL_load_image("Light.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);


	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)(GetScreenWidth()) / (GLfloat)(GetScreenHeight()), 0.1f, 1000.0f);
	// This is the value of the angle in which the cube would rotate
	// It can be increased (clockwise) by pressing Righ Arrow key, 
	// decreased (counter clockwise) by pressing Left Arrow key
	GLfloat theta = 0.0f;
	// The depth value could be increased or decreased by pressing
	// Right Arrow key and Left Arrow key , respectively.
	GLfloat depth = -2.0f;

	bool changeColor = false;	// This value is toggled through key press event

	// 11) Now create our game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer and the depth buffer
		glClearColor(0.25f, 0.51f, 0.51f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		ourShader.Use();

		/*
		* Now we write the trasform stuff
		*/
		glm::mat4 model;
		//model = glm::rotate(model, (GLfloat)glfwGetTime()*0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, /* (GLfloat)glfwGetTime() * */ theta, glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 view;
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, depth));


		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


		GLint isChangeColorLoc = glGetUniformLocation(ourShader.Program, "isChangeColor");
		if (changeColor)	// change the color over the texture
		{
			GLfloat timeValue = glfwGetTime();
			//GLfloat greenValue = (glm::sin(timeValue) / 2) + 0.5f;
			glm::vec3 colorValue = glm::vec3(
				(glm::sin(timeValue) / 2) + 0.5f,
				(glm::sin(timeValue) / 2) + 0.5f,
				(glm::cos(timeValue) / 2) + 0.5f);
			GLint ourColorLoc = glGetUniformLocation(ourShader.Program, "ourColor");
			//glUniform4f(ourColorLoc, 0.0f, greenValue, 0.0f, 1.0f);
			glUniform1i(isChangeColorLoc, true);
			glUniform4fv(ourColorLoc, 1, glm::value_ptr(glm::vec4(colorValue, 1.0f)));
		}
		else		// don't change the color over the texture!!
		{
			glUniform1i(isChangeColorLoc, false);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);

		/**
		* 
		* Handle the key Input
		* 
		*/

		// This will handle the Escape key to easily return to the desktop
		// helpful when making a fullscreen application
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		// Rotate clockwise
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			theta += 0.003f;
		// Rotate counter clockwise
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			theta -= 0.003f;
		// Increase the depth
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			depth += 0.001f;
		// Decrease the depth
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			depth -= 0.001f;
		// Change the color on/off
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			changeColor = !changeColor;

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