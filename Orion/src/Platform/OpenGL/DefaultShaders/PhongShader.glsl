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
out vec3 v_CubeMapCoord;
out vec2 v_TextCoord;
flat out int v_TextSlot;

out vec3 v_FragPos;

uniform mat4 u_ViewProj;
uniform mat4 u_ModelMatrix;

void main()
{
   gl_Position =  u_ViewProj * u_ModelMatrix * vec4(a_Position, 1.0);

   v_Normal =  mat3(transpose(inverse(u_ModelMatrix))) * a_Normal; 
   v_Color = a_Color;
   v_TextCoord = a_TextureCoord;
   v_TextSlot = int(a_TextureSlot);

   v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));

}

#type fragment
#version 450 core



layout(location = 0) out vec4 f_Color;




struct Material {
    sampler2D diffuse;
    sampler2D specular;    
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

in vec3 v_Normal;
in vec4 v_Color;
in vec2 v_TextCoord;
flat in int v_TextSlot;

in vec3 v_FragPos;


uniform sampler2D u_Texture[32];
uniform vec3 u_CameraPos;
uniform Material u_Material;

uniform int u_PointLightCount;
uniform int u_SpotLightCount;

uniform PointLight u_Pointlight;
uniform SpotLight u_Spotlight;
uniform DirectionalLight u_Dirlight;



void main()
{


    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    
    vec4 result = vec4(0.0f);

  
    result += CalcDirectionalLight(u_Dirlight, norm, viewDir);
    result += CalcPointLight(u_Pointlight, norm, v_FragPos, viewDir);
    result += CalcSpotLight(u_Spotlight, norm, v_FragPos, viewDir);


    if(result.a < 0.1f) discard;

   
    f_Color = result  ;

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
    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
     return vec4(vec3(ambient + diffuse + specular), texture(u_Material.diffuse, v_TextCoord).w);

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
        vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TextCoord));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
        vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));

        ambient  *= attenuation;
        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;

         return vec4(vec3(ambient + diffuse + specular), texture(u_Material.diffuse, v_TextCoord).w);

    }
    else
    {
     float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TextCoord));
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
    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, v_TextCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TextCoord));
   
        return vec4(vec3(ambient + diffuse + specular), texture(u_Material.diffuse, v_TextCoord).w);

}  