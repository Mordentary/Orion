#type vertex
#version 450 core


layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in vec2 a_TextureCoord;
layout (location = 4) in float a_TextureSlot;


out vec3 v_Normal;
out vec4 v_Color;
out vec2 v_TextCoord;
flat out int v_TextSlot;

out vec3 v_FragPos;

uniform mat4 u_ViewProj;
uniform mat4 u_ModelMatrix;

void main()
{
   gl_Position =  u_ViewProj * u_ModelMatrix * vec4(a_Position, 1.0);

   v_Normal =  a_Normal;  
   v_Color = a_Color;
   v_TextCoord = a_TextureCoord;
   v_TextSlot = int(a_TextureSlot);

   v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));

}

#type fragment
#version 450 core

layout(location = 0) out vec4 f_Color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

     float constant;
    float linear;
    float quadratic;
};

in vec3 v_Normal;
in vec4 v_Color;
in vec2 v_TextCoord;
flat in int v_TextSlot;

in vec3 v_FragPos;
  
uniform sampler2D u_Texture[32];
uniform vec3 u_CameraPos;
uniform Material material;
uniform Light light;

void main()
{

    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient =  vec3(v_Color) * light.ambient;
  	
    
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
  
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        

        
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;  

    vec4 result = vec4((ambient + diffuse + specular),1.0)* v_Color;

    f_Color = texture(u_Texture[v_TextSlot], v_TextCoord) * result;

} 