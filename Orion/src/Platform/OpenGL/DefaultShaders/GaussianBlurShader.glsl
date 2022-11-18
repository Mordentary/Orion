#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TextureCoord;

uniform mat4 u_ViewProj;

out vec2 v_TextCoord;

void main()
{
	v_TextCoord = a_TextureCoord;
	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 f_Color;


in vec2 v_TextCoord;

uniform sampler2D u_ScreenTex;

uniform bool u_IsHorizontalBlur;
uniform float u_Weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{

    vec2 tex_offset = 1.0 / textureSize(u_ScreenTex, 0); // gets size of single texel
    vec3 result = texture(u_ScreenTex, v_TextCoord).rgb * u_Weights[0]; // current fragment's contribution
    if (u_IsHorizontalBlur)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_ScreenTex, v_TextCoord + vec2(tex_offset.x * i, 0.0)).rgb * u_Weights[i];
            result += texture(u_ScreenTex, v_TextCoord - vec2(tex_offset.x * i, 0.0)).rgb * u_Weights[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_ScreenTex, v_TextCoord + vec2(0.0, tex_offset.y * i)).rgb * u_Weights[i];
            result += texture(u_ScreenTex, v_TextCoord - vec2(0.0, tex_offset.y * i)).rgb * u_Weights[i];
        }
    }

    f_Color = vec4(result, 1.0);
}