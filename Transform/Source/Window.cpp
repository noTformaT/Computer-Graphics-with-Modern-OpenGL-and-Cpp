#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	mainWindow = 0;
	bufferHeight = 0;
	bufferWidth = 0;
}

Window::Window(GLuint windowWidth, GLuint windowHeight):Window()
{
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialise()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization FAILED!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties

    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);
    if (!mainWindow)
    {
        printf("GLFW window creation FAILED!");
        glfwTerminate();
        return 1;
    }

    //// Get buffer size information
    //int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context FOR GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization FAILED!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
}

GLfloat Window::GetBufferWidth()
{
	return bufferWidth;
}

GLfloat Window::GetBufferHeight()
{
	return bufferHeight;
}

bool Window::GetShouldClose()
{
	return glfwWindowShouldClose(mainWindow);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(mainWindow);
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
