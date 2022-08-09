#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextCoord;
layout(location = 3) in float a_TextSlot;



uniform mat4 u_ViewProjection;


out vec2 v_TextCoord;
out vec4 v_Color;
flat out int v_TextSlot;

void main()
{
	v_Color = a_Color;
	v_TextSlot = int(a_TextSlot);
	v_TextCoord = a_TextCoord;
	gl_Position =  u_ViewProjection * vec4(a_Position,1.0);
}


#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TextCoord;
in vec4 v_Color;
flat in int v_TextSlot;

uniform sampler2D u_Texture[32];

void main()
{
	
	color = texture(u_Texture[v_TextSlot], v_TextCoord) * v_Color;
	//color = v_Color;
}