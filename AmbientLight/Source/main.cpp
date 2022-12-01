#define STB_IMAGE_IMPLEMENTATION

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
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

// windows dimentions
const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;
Camera mainCamera;

Texture brickTexture;
Texture dirtTexture;
Texture capTexture;

float currentAngle = 0.0f;
float rotationIncrement = 90.0f;

// Shaders file
static const char* vShader = "../Shaders/vert_UVMapping.glsl";
static const char* fShader = "../Shaders/frag_DirectionLight.glsl";

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;

void CreateObjects()
{
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9,10, 11,
        12,13,14,
        15,16,17,
        18,19,20,
        21,22,23,
        24,25,26,
        27,28,29,
        30,31,32,
        33,34,35
    };

    GLfloat vertices[] = {
        //  x      y      z     u     v
           -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 
            1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, 1.0f,

           -1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
           -1.0f, 1.0f,  1.0f, 0.0f, 1.0f,

            1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
            1.0f,-1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

            1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,  1.0f, 0.0f, 1.0f,

            1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
           -1.0f,-1.0f, -1.0f, 1.0f, 0.0f,
           -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

            1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
           -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f,

           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
           -1.0f,-1.0f,  1.0f, 1.0f, 0.0f,
           -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,

           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
           -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
           -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,

           -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

           -1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
           -1.0f, 1.0f, -1.0f, 0.0f, 1.0f,

           -1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
            1.0f,-1.0f,  1.0f, 1.0f, 0.0f,
            1.0f,-1.0f, -1.0f, 1.0f, 1.0f,

           -1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
            1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
           -1.0f,-1.0f, -1.0f, 0.0f, 1.0f,
    };

    Mesh* mesh1 = new Mesh();
    mesh1->CreateMesh(vertices, indices, 180, 36);
    meshList.push_back(mesh1);

    Mesh* mesh2 = new Mesh();
    mesh2->CreateMesh(vertices, indices, 180, 36);
    meshList.push_back(mesh2);

    Mesh* mesh3 = new Mesh();
    mesh3->CreateMesh(vertices, indices, 180, 36);
    meshList.push_back(mesh3);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialise();

    // Create triangles and shaders
    CreateObjects();
    CreateShaders();

    mainCamera = Camera(glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.0f, .0f), -90.0f, 0.0f, 2.0f, 0.3f);

    brickTexture = Texture((char*)"../Textures/box.jpg");
    brickTexture.LoadTexture(GL_RGB);

    dirtTexture = Texture((char*)"../Textures/dirt.png");
    dirtTexture.LoadTexture();

    capTexture = Texture((char*)"../Textures/Cap.png");
    capTexture.LoadTexture();

    Light mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Calc projection
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);
    GLuint uniformProjection = shaderList[0].GetModelLocation();
    GLuint uniformModel = shaderList[0].GetProjectionLocation();
    GLuint uniformView = shaderList[0].GetViewLocation();
    GLuint uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
    GLuint uniformAmbientColor = shaderList[0].GetAmbientColorLocation();

    // Loop until window closed
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = (GLfloat)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        currentAngle += rotationIncrement * deltaTime;
        if (currentAngle >= 360.0f)
        {
            currentAngle -= 360.0f;
        }
        else 
        {
            if (currentAngle < 0)
            {
                currentAngle += 360.0f;
            }
        }
        

        // Get + Handle User Input
        glfwPollEvents();
        
        mainWindow.UpdateMouse();

        mainCamera.KeyControl(mainWindow.GetKeys(), deltaTime);
        mainCamera.MouseControl(mainWindow.GetMouseXChange(), mainWindow.GetMouseYChange(), deltaTime);

        mainLight.KeyControl(mainWindow.GetKeys(), deltaTime);

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();

        glm::mat4 model(1.0f);

        float scale = .3f;

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(.0f, 1.0f, .0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.CalculateVewMatrix()));
        capTexture.UseTexture();
        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor);
        meshList[0]->RenderMesh();


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.UseTexture();
        meshList[2]->RenderMesh();

        glUseProgram(0);

        mainWindow.SwapBuffers();
    }

    for (size_t i = 0; i < meshList.size(); i++)
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
