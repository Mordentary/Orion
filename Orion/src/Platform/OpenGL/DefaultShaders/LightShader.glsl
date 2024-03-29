#type vertex
#version 450 core


layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec3 a_Tangent;
layout (location = 3) in vec3 a_Bitangent;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in vec2 a_TextureCoord;
layout (location = 6) in float a_TextureSlot;




layout(std140) uniform u_MatricesBuffer
{
    uniform mat4 u_ViewProj;
};

uniform mat4 u_ModelMatrix;

void main()
{
   gl_Position =  u_ViewProj * u_ModelMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core



layout(location = 0) out vec4 f_Color;

  

uniform vec3 u_LightColor;



void main()
{
    
    f_Color =  vec4(u_LightColor,1.0f);

} 

