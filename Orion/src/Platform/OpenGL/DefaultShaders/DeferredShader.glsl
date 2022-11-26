#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TextCoord;


out vec2 v_TextCoord;

uniform mat4 u_DirLightMatrix;
uniform mat4 u_SpotLightMatrix;


void main()
{
    v_TextCoord = a_TextCoord;
    gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 450 core


layout(location = 0) out vec4 f_Color;




struct DirectionalLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};



struct PointLight 
{

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;


    float farPlane;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragLightSpace);
vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragLightSpace);

float ShadowCalculationDir(vec4 shadowFrag,  vec3 normal);
float ShadowCalculationSpot(vec4 shadowFrag, vec3 normal);
float ShadowCalculationPoint(vec3 shadowFrag);



in vec2 v_TextCoord;



uniform sampler2D u_ShadowMapDir;
uniform sampler2D u_ShadowMapSpot;
uniform samplerCube u_ShadowCubemap;

uniform sampler2D u_gPosition;
uniform sampler2D u_gNormal;
uniform sampler2D u_gAlbedoSpec;


uniform vec3 u_CameraPos;


uniform PointLight u_Pointlight;
uniform SpotLight u_Spotlight;
uniform DirectionalLight u_Dirlight;


uniform mat4 u_DirLightMatrix;
uniform mat4 u_SpotLightMatrix;

void main()
{

    vec3 FragPos = texture(u_gPosition, v_TextCoord).rgb;
    vec3 Normal = normalize(texture(u_gNormal, v_TextCoord).rgb);

    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec4 result = vec4(0.0f);


    vec4 FragPosDirLight = u_DirLightMatrix * vec4(FragPos, 1.0f);
    vec4 FragPosSpotLight = u_SpotLightMatrix * vec4(FragPos, 1.0f) ;


    result += CalcPointLight(u_Pointlight, Normal, FragPos, viewDir);
    result += CalcSpotLight(u_Spotlight, Normal, FragPos, viewDir, FragPosSpotLight);
    result += CalcDirectionalLight(u_Dirlight, Normal, viewDir, FragPosDirLight);
    

    f_Color = result;
}

float ShadowCalculationDir(vec4 fragPosLightSpace, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMapDir, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, u_Dirlight.direction)), 0.005f);

    int filterSize = 4;
    int  halfFilterSize = filterSize / 2;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMapDir, 0);
    for (int x = -halfFilterSize; x < -halfFilterSize + filterSize; ++x)
    {
        for (int y = -halfFilterSize; y < -halfFilterSize + filterSize; ++y)
        {
            vec2 offset = vec2(x, y) * texelSize;
            float pcfDepth = texture(u_ShadowMapDir, projCoords.xy + offset).r;

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= float(pow(filterSize, 2));

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float ShadowCalculationSpot(vec4 fragPosLightSpace, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMapSpot, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.01 * (1.0 - dot(normal, u_Spotlight.direction)), 0.2f);

    int filterSize = 5;
    int  halfFilterSize = filterSize / 2;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMapSpot, 0);
    for (int x = -halfFilterSize; x < -halfFilterSize + filterSize; ++x)
    {
        for (int y = -halfFilterSize; y < -halfFilterSize + filterSize; ++y)
        {
            vec2 offset = vec2(x, y) * texelSize;
            float pcfDepth = texture(u_ShadowMapSpot, projCoords.xy + offset).r;

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= float(pow(filterSize, 2));


    return shadow;
}

float ShadowCalculationPoint(vec3 fragPos)
{
    vec3 fragToLight = fragPos - u_Pointlight.position;


    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;
    float samples = 2.0;
    float offset = 0.01;
    for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(u_ShadowCubemap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= u_Pointlight.farPlane;   // undo mapping [0;1]
                if (currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);

    return shadow;


}


vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
   // vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * texture(u_gAlbedoSpec, v_TextCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_gAlbedoSpec, v_TextCoord).rgb;
    vec3 specular = light.specular * spec * texture(u_gAlbedoSpec, v_TextCoord).a;



    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculationPoint(fragPos);

    vec3 finalColor = vec3(diffuse + specular) * (1.f - shadow) + ambient;



    return vec4(finalColor, 1.0f);

}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
{


    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.outerCutOff)
    {

        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
       // vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
        // attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance * distance));
        float epsilon = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        // combine results
        vec3 ambient = light.ambient * texture(u_gAlbedoSpec, v_TextCoord).rgb;
        vec3 diffuse = light.diffuse * diff * vec3(texture(u_gAlbedoSpec, v_TextCoord));
        vec3 specular = light.specular * spec * texture(u_gAlbedoSpec, v_TextCoord).a;


        ambient *= attenuation;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        float shadow = ShadowCalculationSpot(fragPosLightSpace, normal);

        return vec4(vec3(diffuse + specular) * (1.f - shadow) + ambient, 1.0f);

    }
    else
    {
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 ambient = light.ambient * texture(u_gAlbedoSpec, v_TextCoord).rgb;

        ambient *= attenuation;

        return vec4(vec3(ambient), 1.0f);
    }

}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
    // combine results
    vec3 ambient = light.ambient * texture(u_gAlbedoSpec, v_TextCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_gAlbedoSpec, v_TextCoord).rgb;
    vec3 specular = light.specular * spec * texture(u_gAlbedoSpec, v_TextCoord).a;

    float shadow = ShadowCalculationDir(fragPosLightSpace, normal);

    return vec4(vec3(diffuse + specular) * (1.f - shadow) + ambient, 1.0f);

}