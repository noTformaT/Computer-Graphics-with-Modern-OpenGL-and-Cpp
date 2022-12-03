#version 330

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    vec2 fix = vec2(TexCoord.x, 1 - TexCoord.y);
    
    vec4 textureColor = texture(theTexture, fix);

    vec4 uvColor = vec4(TexCoord, .0f, 1.0f);

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity *  diffuseFactor;

    //color = vec4(diffuseFactor, diffuseFactor, diffuseFactor, 1.0f);
    color =  textureColor * (ambientColor + diffuseColor);

    //color =  textureColor * ambientColor;

    //float b = diffuseFactor + ambientColor;
    //color = vec4(ambientColor + diffuseColor);
}