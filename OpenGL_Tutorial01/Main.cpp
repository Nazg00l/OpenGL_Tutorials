
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW\glfw3.h>

#include <iostream>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0 ) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4( position.x, position.y, position.z, 1.0f );\n"
"}\n";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );\n"
"}\n";

// The MAIN function, from here we start the application and run the game loop
int main(int argc, char* argv[])
{
	// Init GLFW
	if (!glfwInit())
	{
		std::cout << "glfwInit() failed\n";
		return -1;
	}

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Tutorial 1", nullptr, nullptr);
	
	/**
	* This is the actual or the accurate representation of what our window is
	* relative to our screen
	*/
	int ScreenWidth, ScreenHeight;
	
	glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);
	
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	
	// ======================================
	/** Create the shaders and its program */
	//=======================================

	// Create a Shader handle of type vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Manipulate the desired shader with the source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the shader after filling it with the source code
	glCompileShader(vertexShader);
	// Check if there was a compilation error
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if ( !success )
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	// Create a Shader handle of type fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Manipulate the desired shader with the source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the shader after filling it with the source code
	glCompileShader(fragmentShader);
	// Check if there was a compilation error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	// Now create our game loop
	while ( !glfwWindowShouldClose(window) )
	{
		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw opengl stuff

		// Swap the screen buffers
		glfwSwapBuffers(window);

	}

	// Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS;

}