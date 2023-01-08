#include <GLFW\glfw3.h>

#include "Win32Window.h"
#include <iostream>


int main(int argc, char* argv[])
{
	GLFWwindow *window = nullptr;
	window = Win32Window::CreateWindow(1024, 768, "OpenGL Tutorial 4 Transformations");

	if (window == nullptr) {
		return EXIT_FAILURE;
	}

	Win32Window::Run(window);

	return EXIT_SUCCESS;
}


