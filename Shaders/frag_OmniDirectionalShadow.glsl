#version 330

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightsCount;
uniform int spotLightsCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;
uniform vec3 eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
    projCoords = (projCoords * 0.5) + 0.5;

    float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction);

    float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }

    //shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

    shadow /= 9.0f;

    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);

    /*float shadow = 0.0f;
    float bias = 0.05;
    float samples = 4.0f;
    float offset = 0.1f;

    for (float x = -offset; x < offset; x += offset / (samples / 0.5))
    {
        for (float y = -offset; y < offset; y += offset / (samples / 0.5))
        {
            for (float z = -offset; z < offset; z += offset / (samples / 0.5))
            {
                float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= omniShadowMaps[shadowIndex].farPlane;
                if (currentDepth - bias > closestDepth)
                {
                    shadow += 1.0f;
                }
            }
        }
    }

    shadow /= (samples * samples * samples);
    return shadow;*/
    
    float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight).r;
    closestDepth *= omniShadowMaps[shadowIndex].farPlane;

    

    float bias = 0.05;

    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    

    vec4 uvColor = vec4(TexCoord, .0f, 1.0f);

    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity *  diffuseFactor;

    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        specularFactor *= light.diffuseIntensity > 1.0f ? 1.0f : light.diffuseIntensity;
        //specularFactor = 0.0f;
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);

    vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);
    float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant;

    return (color / attenuation);
}

vec4 CalcPointLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < pointLightsCount; i++)
    {
        totalColor += CalcPointLight(pointLights[i], i);
    }

    return totalColor;
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);

    if (slFactor > sLight.edge)
    {
        vec4 color = CalcPointLight(sLight.base, shadowIndex);
        return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
    }

    return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

vec4 CalcSpotLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spotLightsCount; i++)
    {
        totalColor += CalcSpotLight(spotLights[i], i + pointLightsCount);
    }

    return totalColor;
}

void main()
{
    //color = CalcPointLights();
    //return;

    vec2 fix = vec2(TexCoord.x, 1 - TexCoord.y);

    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights();

    color = /*texture(theTexture, fix) **/ finalColor;
}