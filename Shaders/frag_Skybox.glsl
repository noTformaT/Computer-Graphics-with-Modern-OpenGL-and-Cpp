#version 330

in vec3 TexCoord;

out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoord);
    //color = vec4(TexCoord, 1.0);
}