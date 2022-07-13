#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
		
uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;

out vec3 v_color;

void main()
{
	v_color = a_Color;
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 330 core

out vec4 color;

in vec3 v_color;

void main()
{
	color = vec4(v_color,1.0);
}
