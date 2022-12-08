#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;
    mainWindow = 0;
    bufferHeight = 0;
    bufferWidth = 0;
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
}

Window::Window(GLuint windowWidth, GLuint windowHeight) :Window()
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

    // Handle keysboard and mouse
    CreateCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //// Get buffer size information
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

    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

GLfloat Window::GetBufferWidth()
{
    return (GLfloat)bufferWidth;
}

GLfloat Window::GetBufferHeight()
{
    return (GLfloat)bufferHeight;
}

bool Window::GetShouldClose()
{
    return glfwWindowShouldClose(mainWindow);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(mainWindow);
}

GLfloat Window::GetMouseXChange()
{
    GLfloat result = xChange;
    xChange = .0f;
    return result;
}

GLfloat Window::GetMouseYChange()
{
    GLfloat result = yChange;
    yChange = .0f;
    return result;
}

bool* Window::GetKeys()
{
    return keys;
}

void Window::UpdateMouse()
{
    double xpos, ypos;
    glfwGetCursorPos(mainWindow, &xpos, &ypos);
    Window::HandleMouse(mainWindow, xpos, ypos);
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

void Window::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, HandleKeys);
    //glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            //printf("Pressed: %d\n", key);
        }
        else
        {
            if (action == GLFW_RELEASE)
            {
                theWindow->keys[key] = false;
                //printf("Released: %d\n", key);
            }
        }
    }
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
    //return;
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = (float)xPos;
        theWindow->lastY = (float)yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = (float)xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - (float)yPos;

    theWindow->lastX = (float)xPos;
    theWindow->lastY = (float)yPos;

    //printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}
 