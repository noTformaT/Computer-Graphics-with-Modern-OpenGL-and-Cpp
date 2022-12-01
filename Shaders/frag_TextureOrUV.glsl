#version 330

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D theTexture;

void main()
{
    //color =  vertexColor;
    vec2 fix = vec2(TexCoord.x, 1 - TexCoord.y);
    color = texture(theTexture, fix);
    //color = vec4(TexCoord.x, TexCoord.y, 0, 1);
}