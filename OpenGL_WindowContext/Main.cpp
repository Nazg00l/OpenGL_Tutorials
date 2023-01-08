#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL\glew.h>

// GLFW
#include <GLFW\glfw3.h>

// 1) Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main(int argc, char* argv[])
{
	
	// 2) Init GLFW
	glfwInit();

	// 3) Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 4) Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Window Context Creation", nullptr, nullptr);

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

		// Swap the screen buffers
		glfwSwapBuffers(window);

		/** 
		* This will handle the Escape key to easily return to the desktop 
		* helpful If you are making a fullscreen application
		*/
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// 12) Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS;

}