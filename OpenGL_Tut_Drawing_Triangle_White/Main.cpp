// Statically link glew and include glew
#define GLEW_STATIC
#include <GL\glew.h>

//inclue the GLFW
#include <GLFW/glfw3.h>

#include <iostream>

// Shader Sources
// Vertex Shader souce code
//const GLchar* vertexSource = R"glsl(
//	#version 150 core
//	in vec2 position;
//	void main()
//	{
//		gl_Position = vec4(position, 0.0, 1.0);
//	}
//)glsl";

const GLchar* vertexSource = "#version 150 core\n"
"in vec2 position;\n"
"void main()\n"
"{\n"
	"gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

// Fragment Shader souce code 
const GLchar* fragmentSource = R"glsl(
	#version 150 core
	
	out vec4 outColor;
	void main()
	{
		outColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
)glsl";

// 1) Window dimensions
GLint WIDTH = 800, HEIGH = 600;

int main(int argc, char* argv[])
{
	// 2) Initialize GLFW
	glfwInit();

	// 3) Set all the required options for GLFW by filling out the window hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 4) Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGH, "OpenGL Drawing White Triangle", nullptr, nullptr);

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

	// Create the vertex array object VAO
	GLuint Vao;
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	// Represent the vertex with the a raw array and Create the vertex buffer object 
	GLfloat vertices[] = {
		0.0f, 0.5f,		// Top vertex
		0.5f, -0.5f,	// Bottom right
		-0.5f, -0.5f	// Bottom left
	};
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint success;			// to get the shader compilation status
	GLchar infoLog[512];	// to store the compilation failure log
	// Create and Compile the VS
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);	// Set the vertex shader source
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	// Create and Compile the FS
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);	// Set the fragment shader source
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	// Link the VS and FS into shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 1, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of Vertex data 
	// or more understandable is :
	// Making the Link Between vertex data and attributes
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);



	// 11) Now create our game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated(key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//
		// Draw opengl stuff
		//
		// Draw the Triangle from the 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		/**
		* This will handle the Escape key to easily return to the desktop
		* helpful If you are making a fullscreen application
		*/
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

	}

	// Since we are done with these , we don't need them any more bye bye!!
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &Vao);

	glfwTerminate();
	return EXIT_SUCCESS;
}