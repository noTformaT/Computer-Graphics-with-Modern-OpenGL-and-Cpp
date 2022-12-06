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
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"
#include "SpotLight.h"

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
Texture planeTexture;

float currentAngle = 0.0f;
float rotationIncrement = 90.0f;

// Shaders file
static const char* vShader = "../Shaders/vert_SpotLight.glsl";
static const char* fShader = "../Shaders/frag_SpotLight.glsl";

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;

void CalcAverageNormals(unsigned int* indices, unsigned int indicesCount, 
    GLfloat* in_vertices, unsigned int verticesCount, 
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indicesCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(in_vertices[in1] - in_vertices[in0], in_vertices[in1 + 1] - in_vertices[in0 + 1], in_vertices[in1 + 2] - in_vertices[in0 + 2]);
        glm::vec3 v2(in_vertices[in2] - in_vertices[in0], in_vertices[in2 + 1] - in_vertices[in0 + 1], in_vertices[in2 + 2] - in_vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        in_vertices[in0] += normal.x; in_vertices[in0 + 1] += normal.y; in_vertices[in0 + 2] += normal.z;
        in_vertices[in1] += normal.x; in_vertices[in1 + 1] += normal.y; in_vertices[in1 + 2] += normal.z;
        in_vertices[in2] += normal.x; in_vertices[in2 + 1] += normal.y; in_vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticesCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        GLfloat X = in_vertices[nOffset];
        GLfloat Y = in_vertices[nOffset + 1];
        GLfloat Z = in_vertices[nOffset + 2];
        glm::vec3 vec(X, Y, Z);
        vec = glm::normalize(vec);
        in_vertices[nOffset] = vec.x; in_vertices[nOffset + 1] = vec.y; in_vertices[nOffset + 2] = vec.z;
    }
}

void CalcNormal(unsigned int* indices, unsigned int indicesCount,
    GLfloat* in_vertices, unsigned int verticesCount,
    unsigned int vLength, unsigned int normalOffset)
{
    /*-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,*/

    for (size_t i = 0; i < indicesCount; i += 3)
    {
        printf("--------------\n");
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 p1 = glm::vec3(in_vertices[in0], in_vertices[in0 + 1], in_vertices[in0 + 2]);
        glm::vec3 p2 = glm::vec3(in_vertices[in1], in_vertices[in1 + 1], in_vertices[in1 + 2]);
        glm::vec3 p3 = glm::vec3(in_vertices[in2], in_vertices[in2 + 1], in_vertices[in1 + 2]);

        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;

        GLfloat x = (u.z * v.z) - (u.z * v.y);
        GLfloat y = (u.z * v.x) - (u.x * v.z);
        GLfloat z = (u.x * v.y) - (u.y * v.x);

        glm::vec3 result = glm::vec3(x, y, z);
        result = glm::normalize(result);

        printf("p1 x: %.0f, y: %.0f, z: %.0f | x: %.0f, y: %.0f, z: %.0f \n", p1.x, p1.y, p1.z, result.x, result.y, result.z);
        printf("p2 x: %.0f, y: %.0f, z: %.0f | x: %.0f, y: %.0f, z: %.0f \n", p2.x, p2.y, p2.z, result.x, result.y, result.z);
        printf("p2 x: %.0f, y: %.0f, z: %.0f | x: %.0f, y: %.0f, z: %.0f \n", p3.x, p3.y, p3.z, result.x, result.y, result.z);
    
        int test = 0;
    }

   

    
}

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
        //  x     y      z     u     v     nx    ny    nz
           -1.0f,-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,-1.0f,
            1.0f,-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,-1.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,-1.0f,

           -1.0f,-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,-1.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,-1.0f,
           -1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,-1.0f,
            //right
            1.0f,-1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
            1.0f,-1.0f, -1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f,

            1.0f,-1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 0.0f, 1.0f,-1.0f, 0.0f, 0.0f,
            //back
            1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
           -1.0f,-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
           -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
           -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            //left
           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           -1.0f,-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           -1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
           -1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
           -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
           // top
           -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f,-1.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f,-1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,-1.0f, 0.0f,

           -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f,-1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,-1.0f, 0.0f,
           -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,-1.0f, 0.0f,
           // bottom
           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,1.0f, 0.0f,
            1.0f,-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f,
            1.0f,-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f,

           -1.0f,-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,1.0f, 0.0f,
            1.0f,-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f,
           -1.0f,-1.0f,  1.0f, 0.0f, 1.0f, 0.0f,1.0f, 0.0f,
    };

    unsigned int indices_pyr[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices_pyr[] = {
        //	x      y      z			u	  v			nx	  ny    nz
        -1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
    };
    
    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,  0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,   10.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,   0.0f, 10.0f,   0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,    10.0f, 10.0f,  0.0f, -1.0f, 0.0f
    };

    //CalcAverageNormals(indices, 36, vertices, 288, 8, 5);

    CalcAverageNormals(indices_pyr, 12, vertices_pyr, 32, 8, 5);

    //CalcNormal(indices, 36, vertices, 288, 8, 5);

    Mesh* mesh1 = new Mesh();
    //mesh1->CreateMesh(vertices_pyr, indices_pyr, 32, 12);
    mesh1->CreateMesh(vertices, indices, 288, 36);
    meshList.push_back(mesh1);

    Mesh* mesh2 = new Mesh();
    mesh2->CreateMesh(vertices, indices, 288, 36);
    meshList.push_back(mesh2);

    Mesh* mesh3 = new Mesh();
    mesh3->CreateMesh(vertices, indices, 288, 36);
    meshList.push_back(mesh3);

    Mesh* mesh4 = new Mesh();
    mesh4->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(mesh4);
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
    //mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();

    // Create triangles and shaders
    CreateObjects();
    CreateShaders();

    mainCamera = Camera(glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.0f, .0f), -90.0f, 0.0f, 2.0f, 0.3f);

    brickTexture = Texture((char*)"../Textures/box.jpg");
    brickTexture.LoadTexture(GL_RGB);

    dirtTexture = Texture((char*)"../Textures/dirt.png");
    dirtTexture.LoadTexture();

    capTexture = Texture((char*)"../Textures/cap.png");
    capTexture.LoadTexture();

    planeTexture = Texture((char*)"../Textures/plain.png");
    planeTexture.LoadTexture();

    DirectionalLight mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                                  0.0f, 0.0f,
                                                  0.0f, 0.0f, -1.0f);

    unsigned int pointLightCount = 0;

    PointLight pointLights[MAX_POINT_LIGHTS];
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.1f, 1.0f, 
                                4.0f, 0.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
                                0.1f, 1.0f,
                                -4.0f, 0.0f, 0.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;

    unsigned int spotLightCount = 0;
    SpotLight spotLights[MAX_SPOT_LIGHTS];
      
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        -1.0f, -0.7f, 0.0f,
        0.1f, 0.2f, 0.1f,
        20.0f);
    spotLightCount++;

    spotLights[1] = SpotLight(1.0f, 0.5f, 0.0f,
        0.0f, 2.0f,
        0.0f, -1.5f, 0.0f,
        0.0f, -1.0f, -5.0f,
        0.1f, 0.2f, 0.1f,
        20.0f);
    spotLightCount++;

    spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f,
        0.0f, 2.0f,
        0.0f, -0.0f, 3.0f,
        0.0f, -1.0f, 0.0f,
        0.1f, 0.2f, 0.1f,
        20.0f);
    spotLightCount++;

    Material shinyMaterial = Material(4.0f, 256);
    Material dullMaterial = Material(0.00f, 4);
    
    // Calc projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;

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
        GLfloat bgColor = mainLight.GetAmbientIntensity();
        glClearColor(bgColor, bgColor, bgColor, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glm::vec3 lightPos = mainCamera.GetCameraPosition();
        lightPos.y -= -0.1f;
        //spotLights[0].SetPositionAndDirection(lightPos, mainCamera.GetCameraDirection());
        spotLights[0].AddDegress(45.0f * deltaTime * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        spotLights[2].AddDegress(90.0f * deltaTime * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        //mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.CalculateVewMatrix()));
        
        glm::vec3 cameraPosition = mainCamera.GetCameraPosition();
        glUniform3f(uniformEyePosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
        
        glm::mat4 model(1.0f);

        float scale = .3f;

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(.0f, 1.0f, .0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        capTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        model = glm::rotate(model, -currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, -0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        //planeTexture.UseTexture();
        //shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[3]->RenderMesh();

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
