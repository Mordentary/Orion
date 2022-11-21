#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec3 a_Tangent;
layout (location = 3) in vec3 a_Bitangent;
layout (location = 4) in vec4 a_Color;
layout (location = 5) in vec2 a_TextureCoord;
layout (location = 6) in float a_TextureSlot;


uniform mat4 u_ViewProj;

out vec3 v_CubeMapCoord;

void main()
{
	v_CubeMapCoord = a_Position;
	gl_Position = vec4(u_ViewProj  * vec4(a_Position,1.0)).xyww;
}


#type fragment
#version 450 core
			
layout(location = 0) out vec4 f_Color;

in vec3 v_CubeMapCoord;
uniform samplerCube u_Cubemap;




const float gamma = 2.2f;

void main()
{
		f_Color = vec4(texture(u_Cubemap, v_CubeMapCoord));

}