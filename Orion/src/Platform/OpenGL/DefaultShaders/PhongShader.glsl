#type vertex
#version 450 core


layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec3 a_Tangent;
layout (location = 3) in vec3 a_Bitangent;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in vec2 a_TextureCoord;
layout (location = 6) in float a_TextureSlot;




out vec3 v_Normal;
out vec4 v_Color;
out vec2 v_TextCoord;
out vec3 v_FragPos;
out vec4 v_FragPosDirLight;
out vec4 v_FragPosSpotLight;
out mat3 v_TBN;





uniform mat4 u_DirLightMatrix;
uniform mat4 u_SpotLightMatrix;

uniform mat4 u_ModelMatrix;

layout(std140) uniform u_MatricesBuffer
{
    uniform mat4 u_ViewProj;
};

void main()
{

   v_Normal =  mat3(transpose(inverse(u_ModelMatrix))) * a_Normal; 
   v_Color = a_Color;
   v_TextCoord = a_TextureCoord;


   vec3 T = normalize(vec3(u_ModelMatrix * vec4(a_Tangent, 0.0)));
   vec3 B = normalize(vec3(u_ModelMatrix * vec4(a_Bitangent, 0.0)));
   vec3 N = normalize(vec3(u_ModelMatrix * vec4(a_Normal, 0.0)));

   v_TBN = mat3(T, B, N);

   v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
   v_FragPosDirLight = u_DirLightMatrix * vec4(v_FragPos, 1.0);
   v_FragPosSpotLight = u_SpotLightMatrix * vec4(v_FragPos, 1.0);

   gl_Position =  u_ViewProj * vec4(v_FragPos, 1.0);


}

#type fragment
#version 450 core



layout(location = 0) out vec4 f_Color;
layout(location = 1) out vec4 f_BrightColor;





struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normals;

    float shininess;
}; 



struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};



struct PointLight {

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
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

float ShadowCalculationDir(vec4 shadowFrag, vec3 normal);
float ShadowCalculationSpot(vec4 shadowFrag, vec3 normal);
float ShadowCalculationPoint(vec3 shadowFrag);

in vec3 v_Normal;
in vec4 v_Color;
in vec2 v_TextCoord;
in vec3 v_FragPos;
in vec4 v_FragPosDirLight;
in vec4 v_FragPosSpotLight;

in mat3 v_TBN;


uniform sampler2D u_ShadowMapDir;
uniform sampler2D u_ShadowMapSpot;
uniform samplerCube u_ShadowCubemap;

uniform vec3 u_CameraPos;
uniform Material u_Material;

uniform PointLight u_Pointlight;
uniform SpotLight u_Spotlight;
uniform DirectionalLight u_Dirlight;


void main()
{

    vec3 normal = texture(u_Material.normals, v_TextCoord).rgb;
    if(normal != vec3(1.0f))
    {
        normal = normal * 2.0 - 1.0;
        normal = normalize(v_TBN * normal);;
    }
    else 
    {
        normal = normalize(v_Normal);
    }
    
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    vec4 result = vec4(0.0f);


    result += CalcPointLight(u_Pointlight, normal, v_FragPos, viewDir);
    result += CalcSpotLight(u_Spotlight, normal, v_FragPos, viewDir);
    result += CalcDirectionalLight(u_Dirlight, normal, viewDir);


    if(result.a < 0.1f) discard;

    f_Color = vec4(result) * v_Color;

    
    f_BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

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

     if(projCoords.z > 1.0)
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
    shadow /= float(pow(filterSize,2));


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

    float spec = pow(max(dot(normal, halfwayDir), 0.0),  u_Material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculationPoint(fragPos);
 
    vec3 finalColor = vec3(diffuse + specular) * (1.0f - shadow) + ambient;

 

    return vec4(finalColor, texture(u_Material.diffuse, v_TextCoord).w);

}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{


    vec3 lightDir = normalize(light.position - fragPos);
   float theta = dot(lightDir, normalize(-light.direction));
     
    if(theta > light.outerCutOff) 
    {
        
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
       // vec3 reflectDir = reflect(-lightDir, normal);
         vec3 halfwayDir = normalize(lightDir + viewDir);

        float spec = pow(max(dot(normal, halfwayDir), 0.0),  u_Material.shininess);
        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			         light.quadratic * (distance * distance));    
        float epsilon   = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
        // combine results
        vec3 ambient  = light.ambient;
        vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
        vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));


        ambient  *= attenuation;
        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;

        float shadow = ShadowCalculationSpot(v_FragPosSpotLight, normal);

         return vec4(vec3(diffuse + specular) * (1.f-shadow) + ambient, texture(u_Material.diffuse, v_TextCoord).w);

    }
    else
    {
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  

        vec3 ambient  = light.ambient;

        ambient  *= attenuation;
   
        return vec4(vec3(ambient), texture(u_Material.diffuse, v_TextCoord).w);
    }
    
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
      vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0),  u_Material.shininess);
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));

    float shadow = ShadowCalculationDir(v_FragPosDirLight, normal);

    return vec4(vec3(diffuse + specular) * (1.f-shadow) + ambient, texture(u_Material.diffuse, v_TextCoord).w);

}  