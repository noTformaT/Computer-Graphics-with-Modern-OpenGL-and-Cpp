#version 330
// В первом атрибуте хранится x,y,z значение точки
layout (location = 0) in vec3 pos;

// В во втором атрибуте хранится u,v значение точки
layout (location = 1) in vec2 tex;

layout (location = 2) in vec3 norm;

out vec4 vertexColor;

// UV координаты которые уйдут из vertex shader в fragment shader
out vec2 TexCoord;

// Normal вектор который уйдет из vertex shader в fragment shader
out vec3 Normal;

// входящие uniform значения матриц проектции, модели и камеры
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    // перевод из local space в camera space
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    
    vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
    
    // вывод в fragment shader uv координат
    TexCoord = tex;

    Normal = mat3(transpose(inverse(model))) * norm;
}