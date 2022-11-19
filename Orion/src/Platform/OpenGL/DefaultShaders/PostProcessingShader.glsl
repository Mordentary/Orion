#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TextCoord;


out vec2 v_TextCoord;

void main()
{
    v_TextCoord = a_TextCoord;
    gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 f_Color;


in vec2 v_TextCoord;

uniform sampler2D u_GaussianBlurTex;
uniform sampler2D u_SceneTex;

uniform bool u_Bloom;

uniform bool u_GammaCorrection;
uniform bool u_HDR;


uniform float u_ExposureFactor;

const float gamma = 2.2;

void main()
{
    vec3 result;

    vec3 hdrColor = texture(u_SceneTex, v_TextCoord).rgb;

    if (u_Bloom) 
    {
        vec3 bloomColor = texture(u_GaussianBlurTex, v_TextCoord).rgb;
        hdrColor += bloomColor; // additive blending
    }
    if (u_HDR) 
    {
        // tone mapping
        result = vec3(1.0) - exp(-hdrColor * u_ExposureFactor);
    }
    else
    {
        result = hdrColor * u_ExposureFactor;
    }

    if (u_GammaCorrection) 
    {
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
    }
   
    f_Color = vec4(result, 1.0);
}
