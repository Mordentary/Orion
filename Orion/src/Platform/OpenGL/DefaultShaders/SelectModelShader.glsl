#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TextureCoord;
layout(location = 6) in float a_TextureSlot;

uniform mat4 u_ViewProj;
uniform mat4 u_ModelMatrix;


out vec2 v_TextCoord;

void main()
{

    v_TextCoord = a_TextureCoord;
    gl_Position = (u_ViewProj * u_ModelMatrix * vec4(a_Position + a_Normal * 0.01, 1.0));
}

#type fragment
#version 450 core



layout(location = 0) out vec4 f_Color;



struct Material
{
    sampler2D diffuse;
};

in vec2 v_TextCoord;
uniform Material u_Material;

void main()
{
    if (texture(u_Material.diffuse, v_TextCoord).a < 0.1f) discard;

    f_Color = vec4(0.7f, 0.25f, 0.25f,1.0f);

}

