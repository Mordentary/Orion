#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
		
uniform mat4 u_ViewProjection;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 330 core

out vec4 color;

uniform vec3 u_Color;

void main()
{
	color = vec4(u_Color,1.0);
}
