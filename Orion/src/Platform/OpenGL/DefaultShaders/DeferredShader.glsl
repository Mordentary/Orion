#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TextCoord;


out vec2 v_TextCoord;



void main()
{
    v_TextCoord = a_TextCoord;
    gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 450 core


layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec4 f_Bloom;





struct DirectionalLight 
{
    vec3 ambient; //4+
    vec3 diffuse; //4+
    vec3 specular; //4+

    vec3 position; //4+
    vec3 direction; //4+

    mat4 VPMatrix;
};



struct PointLight 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float farPlane;
    float radius;
};

struct SpotLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float outerCutOff;
    float innerCutOff;

    mat4 VPMatrix;
};

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragLightSpace);
vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir ,  vec4 fragLightSpace);

float ShadowCalculationDir(vec4 shadowFrag,  vec3 normal);
float ShadowCalculationPoint(PointLight light, vec3 shadowFrag);
float ShadowCalculationSpot (SpotLight light, vec4 shadowFrag, vec3 normal);

float DistributionGGX(vec3 N, vec3 H, float a);
float GeometrySchlickGGX(float NdotV, float k);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k);
vec3 FresnelSchlick(float cosTheta, vec3 F0);




in vec2 v_TextCoord;



uniform sampler2D u_ShadowMapDir;
uniform sampler2D u_ShadowMapSpot;
uniform samplerCube u_ShadowCubemap;

uniform sampler2D u_gPosition_Rougness;
uniform sampler2D u_gNormals;
uniform sampler2D u_gAlbedo_Metallic;
uniform sampler2D u_gEmissionAO;


uniform vec3 u_CameraPos;

#define PI 3.1415926535897932384626433832795
#define MAX_LIGHTS 100

layout(std140) uniform u_LightSources
{

  PointLight u_PointLights[MAX_LIGHTS];
  SpotLight  u_SpotLights[MAX_LIGHTS];
  DirectionalLight u_DirLight; 

  int PointLightCount;
  int SpotLightCount;
  int DirLightCount;

};


void main()
{



    vec3 FragPos = texture(u_gPosition_Rougness, v_TextCoord).rgb;
    vec3 Normal = normalize(texture(u_gNormals, v_TextCoord).rgb);

    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec4 result = vec4(0.0f);


    for (int i = 0; i < PointLightCount; i++)
    {
        float distance = length(u_PointLights[i].position - FragPos);
           // if (distance < u_PointLights[i].radius)
        result += CalcPointLight(u_PointLights[i], Normal, FragPos, viewDir);
    }
    for (int i = 0; i < SpotLightCount; i++)
    {
        vec4 FragPosSpotLight = u_SpotLights[i].VPMatrix * vec4(FragPos, 1.0f);
        result += CalcSpotLight(u_SpotLights[i], Normal, FragPos, viewDir, FragPosSpotLight);
    }
        
    vec4 FragPosDirLight = u_DirLight.VPMatrix * vec4(FragPos, 1.0f);
    result += CalcDirectionalLight(u_DirLight, Normal, viewDir, FragPosDirLight);
    


    f_Color = result;
    f_Bloom = vec4(texture(u_gEmissionAO, v_TextCoord).rgb,1.0f);
}


float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}





vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{


    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    vec3 radiance = light.diffuse * attenuation;

    float mettalic = texture(u_gAlbedo_Metallic, v_TextCoord).a;
    float roughness = texture(u_gPosition_Rougness, v_TextCoord).a;
    vec3 albedoColor = texture(u_gAlbedo_Metallic, v_TextCoord).rgb * texture(u_gEmissionAO, v_TextCoord).a;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedoColor, mettalic);

    float NDF = DistributionGGX(normal, halfwayDir, roughness);
    vec3 F = FresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - mettalic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 outgoingLight = texture(u_gEmissionAO, v_TextCoord).rgb + ((kD * albedoColor / PI + specular) * radiance * NdotL);

    vec3 ambient = vec3(0.03) * albedoColor * attenuation;

    float shadow = ShadowCalculationPoint(light, fragPos);
   // outgoingLight *= (1.0 - shadow);

    vec3 finalColor = ambient + outgoingLight;


    return vec4(finalColor, 1.0f);

}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragLightSpace)
{
    

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));

    vec3 albedoColor = texture(u_gAlbedo_Metallic, v_TextCoord).rgb * texture(u_gEmissionAO, v_TextCoord).a;


    if (theta >= light.outerCutOff)
    {
        float epsilon = (light.innerCutOff - light.outerCutOff);
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        vec3 radiance = light.diffuse * attenuation * intensity;

        float mettalic = texture(u_gAlbedo_Metallic, v_TextCoord).a;
        float roughness = texture(u_gPosition_Rougness, v_TextCoord).a;

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedoColor, mettalic);

        float NDF = DistributionGGX(normal, halfwayDir, roughness);
        vec3 F = FresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
        float G = GeometrySmith(normal, viewDir, lightDir, roughness);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - mettalic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(normal, lightDir), 0.0);

        vec3 outgoingLight = texture(u_gEmissionAO, v_TextCoord).rgb + (kD * albedoColor / PI + specular) * radiance * NdotL;

        vec3 ambient = vec3(0.03) * albedoColor * attenuation;

        float shadow = ShadowCalculationSpot(light, fragLightSpace, normal);

        // outgoingLight *= (1.0 - shadow);

        vec3 finalColor = ambient + outgoingLight;

        return vec4(finalColor, 1.0f);
    }
    else
    {
        vec3 ambient = vec3(0.03) * albedoColor * texture(u_gEmissionAO, v_TextCoord).a;
        return vec4(ambient * attenuation, 1.0f);
    }


}



vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 fragLightSpace)
{

    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 radiance = light.diffuse;

    float mettalic = texture(u_gAlbedo_Metallic, v_TextCoord).a;
    float roughness = texture(u_gPosition_Rougness, v_TextCoord).a;
    vec3 albedoColor = texture(u_gAlbedo_Metallic, v_TextCoord).rgb * texture(u_gEmissionAO, v_TextCoord).a;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedoColor, mettalic);

    float NDF = DistributionGGX(normal, halfwayDir, roughness);
    vec3 F = FresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - mettalic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 outgoingLight = texture(u_gEmissionAO, v_TextCoord).rgb + (kD * albedoColor / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedoColor;

    float shadow = ShadowCalculationDir(fragLightSpace, normal);
   // outgoingLight *= (1.0 - shadow);

    vec3 finalColor = ambient + outgoingLight;

    return vec4(finalColor, 1.0f);
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
    float bias = max(0.01 * (1.0 - dot(normal, u_DirLight.direction)), 0.005f);

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

float ShadowCalculationSpot(SpotLight light, vec4 fragPosLightSpace, vec3 normal)
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
    float bias = max(0.01 * (1.0 - dot(normal, light.direction)), 0.01f);

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

float ShadowCalculationPoint(PointLight light, vec3 fragPos)
{
    vec3 fragToLight = fragPos - light.position;


    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.1;
    float samples = 2.0;
    float offset = 0.01;

    for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(u_ShadowCubemap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= light.farPlane;   // undo mapping [0;1]
                if (currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);

    return shadow;


}

