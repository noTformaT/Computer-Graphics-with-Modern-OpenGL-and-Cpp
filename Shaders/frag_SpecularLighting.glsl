#version 330

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;

void main()
{
    //color = vec4(FragPos, 1.0f);
    //return;
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    vec2 fix = vec2(TexCoord.x, 1 - TexCoord.y);
    
    vec4 textureColor = texture(theTexture, fix);

    vec4 uvColor = vec4(TexCoord, .0f, 1.0f);

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity *  diffuseFactor;

    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    color =  textureColor * (ambientColor + diffuseColor + specularColor);
}