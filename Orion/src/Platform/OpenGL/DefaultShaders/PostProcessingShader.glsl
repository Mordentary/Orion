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


uniform int u_CurrentToneMappingModel;


uniform float u_WhitePoint;
uniform float u_ExposureFactor;
uniform float u_GammaFactor;





float RGBtoLuminance(vec3 v)
{
    return dot(v, vec3(0.2126f, 0.7152f, 0.0722f));
}
vec3 ApplyLuminanceToRGB(vec3 c_in, float l_out)
{
    float l_in = RGBtoLuminance(c_in);

    return c_in * (l_out / l_in);
}

vec3 ACES_Narkowicz(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 Uncharted2Tonemap(vec3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 HableFilmic(vec3 v)
{
 
    vec3 curr = Uncharted2Tonemap(v);

    vec3 W = vec3(11.2f);
    vec3 white_scale = vec3(1.0f) / Uncharted2Tonemap(W);
    return curr * white_scale;
}


vec3 Reinhard(vec3 v) 
{
    return v / (1.0f + v);
}

vec3 ReinhardExt(vec3 v, float max_white)
{
    vec3 numerator = v * (1.0f + (v / vec3(max_white * max_white)));
    return numerator / (1.0f + v);
}

vec3 ReinhardExtluminance(vec3 v, float max_white_l)
{
    float l_old = RGBtoLuminance(v);
    float numerator = l_old * (1.0f + (l_old / (max_white_l * max_white_l)));
    float l_new = numerator / (1.0f + l_old);
    return ApplyLuminanceToRGB(v, l_new);
}
vec3 ReinhardJodie(vec3 v)
{
    float l = RGBtoLuminance(v);
    vec3 tv = v / (1.0f + v);
    return mix(v / (1.0f + l), tv, tv);
}

void main()
{
    vec3 result;

    vec3 hdrColor = texture(u_SceneTex, v_TextCoord).rgb;

    if (u_Bloom) 
    {
        vec3 bloomColor = texture(u_GaussianBlurTex, v_TextCoord).rgb;
        hdrColor += bloomColor; 
    }

    if (u_HDR) 
    {
        if(u_CurrentToneMappingModel == 0)
            result = ACES_Narkowicz(hdrColor * u_ExposureFactor);

        else if (u_CurrentToneMappingModel == 1)
            result = Reinhard(hdrColor * u_ExposureFactor);

        else if (u_CurrentToneMappingModel == 2)
            result = ReinhardExt(hdrColor * u_ExposureFactor, u_WhitePoint);

        else if (u_CurrentToneMappingModel == 3)
            result = ReinhardExtluminance(hdrColor * u_ExposureFactor, u_WhitePoint);

        else if(u_CurrentToneMappingModel == 4)
            result = ReinhardJodie(hdrColor * u_ExposureFactor);

        else if (u_CurrentToneMappingModel == 5)
            result = HableFilmic(hdrColor * u_ExposureFactor);
    }
    else
    {
        result = hdrColor * u_ExposureFactor;
    }
    if (u_GammaCorrection) 
    {
             
        result = pow(result, vec3(1.0 / u_GammaFactor));
    }

   
    f_Color = vec4(result, 1.0);
}
