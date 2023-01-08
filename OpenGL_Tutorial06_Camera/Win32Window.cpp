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
#include "Camera.h"

#include <iostream>

// -------------------->> NOTE <<----------------------
//
// Because m_ScreenWidth, m_ScreenHeight ,and m_Title are static members as declared in Win32Window.h
// they should be defined somewhere and without repeating the word static
// and also we initialized them btw

GLint Win32Window::m_ScreenWidth = 1;
GLint Win32Window::m_ScreenHeight = 1;
const char* Win32Window::m_Title = "";

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement(); 

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// These two may cause a problem since I am using the value of width/height
// that was obtained from glfwGetFramebufferSize(...)
GLfloat lastX = Win32Window::GetScreenWidth() / 2.0f;
GLfloat lastY = Win32Window::GetScreenWidth() / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

	/**
	* Now setting the callbacks
	*/
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// This vertices are to be used with Perspective projection
	
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

	// Used to define the translation for each cube that is drawn
	glm::vec3 cubPositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.0f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(-1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
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
	unsigned char* image = SOIL_load_image("Water.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// This is the value of the angle in which the cube would rotate
	// It can be increased (clockwise) by pressing Righ Arrow key, 
	// decreased (counter clockwise) by pressing Left Arrow key
	GLfloat theta = 0.0f;
	// The depth value could be increased or decreased by pressing
	// Right Arrow key and Left Arrow key , respectively.
	GLfloat depth = -2.0f;

	glm::vec3 rotationAxis[10];
	for (int i = 0; i < 10; ++i)
	{
		rotationAxis[i] = glm::vec3((GLfloat)rand() / RAND_MAX, (GLfloat)rand() / RAND_MAX, (GLfloat)rand() / RAND_MAX);
	}

	// 11) Now create our game loop
	while (!glfwWindowShouldClose(window))
	{
		/**
		* Now set the frame time
		*/
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;	// The time elapsed between frames
		lastFrame = currentFrame;

		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Handle the movenent
		DoMovement();

		// Render
		// Clear the colorbuffer and the depth buffer
		//glClearColor(0.25f, 0.51f, 0.51f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		ourShader.Use();


		/*
		* Now we write the trasform stuff
		*/

		// I have put the porjection inside the while loop, because we are going to 
		// be changing the field of view(FOV) using the mouse scroll, so we want to 
		// be able to update the projection every single frame
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)(GetScreenWidth()) / (GLfloat)(GetScreenHeight()), 0.1f, 1000.0f);

		glm::mat4 model;
		//model = glm::rotate(model, /* (GLfloat)glfwGetTime() * */ theta, glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 view;
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, depth));
		view = camera.GetViewMatrix();

		theta = 0.33f;
		//model = glm::rotate(model,  60.0f, glm::vec3(1.0f, 0.5f, 0.0f));

		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		/*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 5, 36);*/

		glBindVertexArray(VAO);
		
		for (GLuint i = 0; i < 10; ++i)
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(cubPositions[i]));
			GLfloat angle = 10.0f * (i + 1);
			//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, angle * theta * (GLfloat)glfwGetTime(), glm::vec3(1.0f * (GLfloat)rand(), 0.3f * (GLfloat)rand(), 0.5f * (GLfloat)rand()));
			model = glm::rotate(model,  theta  * (GLfloat)glfwGetTime(), rotationAxis[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
		/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);*/
		// Rotate clockwise
		/*if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			theta += 0.003f;*/
		// Rotate counter clockwise
		/*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			theta -= 0.003f;*/
		// Increase the depth
		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			depth += 0.001f;*/
		// Decrease the depth
		/*if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			depth -= 0.001f;*/

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


// Handling our method and calling the appropriate methods in Camera class
void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;

		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;		// This is reversed because the y-coordinates start from the bottom left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}