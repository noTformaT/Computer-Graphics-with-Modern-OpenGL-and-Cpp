#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

// windows dimentions
const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;

float currentAngle = 0.0f;
float rotationIncrement = 1.0f;

// Shaders file
static const char* vShader = "../Shaders/vert_ModelAndProjection.glsl";
static const char* fShader = "../Shaders/frag_VertexColor.glsl";

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -0.86f, -0.7f, -.5f,
        0.0f, -0.7f, 1.0f,
        0.86f, -0.7f, -0.5f,
        0.0f, 0.7f, 0.0f
    };

    Mesh* mesh1 = new Mesh();
    mesh1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh1);

    Mesh* mesh2 = new Mesh();
    mesh2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh2);

    Mesh* mesh3 = new Mesh();
    mesh3->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh3);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader, fShader);
    shaderList.push_back(*shader1);
}

void Render(glm::mat4& projection)
{
    shaderList[0].UseShader();
    GLuint uniformProjection = shaderList[0].GetModelLocation();
    GLuint uniformModel = shaderList[0].GetProjectionLocation();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.f, .0f, 5.0f));
    model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, .1f, .0f));
    model = glm::scale(model, glm::vec3(.4f, .4f, .4f));


    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    meshList[0]->RenderMesh();
}


int main()
{
    mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialise();

    // Create triangles and shaders
    CreateObjects();
    CreateShaders();
    
    // Calc projection
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);
    GLuint uniformProjection = shaderList[0].GetModelLocation();
    GLuint uniformModel = shaderList[0].GetProjectionLocation();

    // Loop until window closed
    while (!mainWindow.GetShouldClose())
    {
        currentAngle += rotationIncrement;

        // Get + Handle User Input
        glfwPollEvents();

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(.0f, 1.0f, .0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(1.0f, 1.0f, .0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[2]->RenderMesh();

        glUseProgram(0);

        mainWindow.SwapBuffers();
    }

    for (int i = 0; i < meshList.size(); i++)
    {
        delete meshList[i];
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
