#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// windows dimentions
const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

GLuint VBO, VAO, IBO, shader, uniformModel;

bool direction = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.005f;
float currentAngle = 0.0f;
float rotationIncrement = 1.0f;
bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
float sizeIncrement = 0.005f;

// Vertex Shader
static const char* vShader = "                                         \n\
#version 330                                                           \n\
layout (location = 0) in vec3 pos;                                     \n\
                                                                       \n\
out vec4 vertexColor;                                                  \n\
                                                                       \n\
uniform mat4 model;                                                    \n\
                                                                       \n\
void main()                                                            \n\
{                                                                      \n\
    gl_Position = model * vec4(pos, 1.0f);                             \n\
    vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                  \n\
}";

// Fragment Shader
static const char* fShader = "                      \n\
#version 330                                        \n\
                                                    \n\
in vec4 vertexColor;                                \n\
                                                    \n\
out vec4 color;                                     \n\
                                                    \n\
void main()                                         \n\
{                                                   \n\
    color =  vertexColor;                           \n\
}                                                   \n\
";

void CreateTriangle()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, code, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar errorLog[128] = { 0 };

    // compile programm
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling the %d shader: %s\n", shaderType, errorLog);
        return;
    }

    glAttachShader(program, theShader);
}

void CompileShaders()
{
    // make program
    shader = glCreateProgram();
    
    if (!shader)
    {
        printf("Error: Creating shader program!\n");
        return;
    }

    // add shader code to programm
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[128] = { 0 };

    // compiling programm
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking program: %s\n", errorLog);
        return;
    }

    // validation programm
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error validation program: %s\n", errorLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");

}

int main()
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
    if (!mainWindow)
    {
        printf("GLFW window creation FAILED!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
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

    // Create triangles and shaders
    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and hanlde user input events
        glfwPollEvents();

        if (direction)
        {
            triangleOffset += triangleIncrement;
        }
        else
        {
            triangleOffset -= triangleIncrement;
        }

        if (abs(triangleOffset) >= triangleMaxOffset)
        {
            direction = !direction;
        }

        currentAngle += rotationIncrement;
        if (currentAngle >= 360.0f)
        {
            currentAngle -= 360.0f;
        }

        if (sizeDirection)
        {
            currentSize += sizeIncrement;
        }
        else
        {
            currentSize -= sizeIncrement;
        }

        if (currentSize >= maxSize || currentSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model = glm::mat4(1.0f);

        //model = glm::translate(model, glm::vec3(triangleOffset, .0f, .0f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(.2f, .1f, 0.1f));
        //model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
        model = glm::scale(model, glm::vec3(.4f, .4f, 1.0f));
        
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
