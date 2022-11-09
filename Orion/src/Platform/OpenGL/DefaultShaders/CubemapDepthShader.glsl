#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TextureCoord;
layout(location = 6) in float a_TextureSlot;

uniform mat4 u_ModelMatrix;
out vec2 v_TextCoord;

void main()
{
    v_TextCoord = a_TextureCoord;
    gl_Position = u_ModelMatrix * vec4(a_Position, 1.0);
}  

#type geometry
#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_ViewProjMatrices[6];

out vec4 g_FragPos; // FragPos from GS (output per emitvertex)

in vec2 v_TextCoord[];
out vec2 g_TextCoord;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            g_FragPos = gl_in[i].gl_Position;
            gl_Position = u_ViewProjMatrices[face] * g_FragPos;
            g_TextCoord = v_TextCoord[i];
            EmitVertex();
        }    
        EndPrimitive();
    }
}

#type fragment
#version 450 core

in vec4 g_FragPos;

uniform vec3 u_LightPos;
uniform float u_FarPlane;

struct Material
{
    sampler2D diffuse;
};

in vec2 g_TextCoord;
uniform Material u_Material;


void main()
{


    if (texture(u_Material.diffuse, g_TextCoord).a < 0.1f) discard;
    // get distance between fragment and light source
    float lightDistance = length(g_FragPos.xyz - u_LightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / u_FarPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}