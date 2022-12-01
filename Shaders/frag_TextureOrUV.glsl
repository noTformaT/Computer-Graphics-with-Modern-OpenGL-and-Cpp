#version 330

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D theTexture;

void main()
{
    //color =  vertexColor;
    //color = texture(theTexture, TexCoord);
    color = vec4(TexCoord, 0, 1);
}