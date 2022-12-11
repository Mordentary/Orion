#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TextureCoord;


out vec3 v_Normal;
out vec4 v_Color;
out vec2 v_TextCoord;
out vec3 v_FragPos;
out mat3 v_TBN;

layout(std140) uniform u_MatricesBuffer
{
    uniform mat4 u_ViewProj;
};

uniform mat4 u_ModelMatrix;

void main()
{

    v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
    v_Color = a_Color;
    v_TextCoord = a_TextureCoord;


    vec3 T = normalize(vec3(u_ModelMatrix * vec4(a_Tangent, 0.0)));
    vec3 B = normalize(vec3(u_ModelMatrix * vec4(a_Bitangent, 0.0)));
    vec3 N = normalize(vec3(u_ModelMatrix * vec4(a_Normal, 0.0)));

    v_TBN = mat3(T, B, N);

    v_FragPos = vec3(u_ModelMatrix * vec4(a_Position, 1.0));

    gl_Position = u_ViewProj * vec4(v_FragPos, 1.0);


}

#type fragment
#version 450 core


layout(location = 0) out vec4 f_Position_Rougness;
layout(location = 1) out vec4 f_Normals;
layout(location = 2) out vec4 f_Albedo_Metallic;
layout(location = 3) out vec4 f_EmissionAO;








struct Material 
{
    sampler2D albedo;
    sampler2D roughness;
    sampler2D mettalic;
    sampler2D normals;
    sampler2D emission;
    sampler2D ao;
};



in vec3 v_Normal; 
in vec4 v_Color;
in vec2 v_TextCoord;
in vec3 v_FragPos;
in mat3 v_TBN;

uniform Material u_Material;


void main()
{
    if (texture(u_Material.albedo, v_TextCoord).a < 0.1f) discard;

    vec3 normal = texture(u_Material.normals, v_TextCoord).rgb;
    if (normal != vec3(1.0f))
    {
        normal = normal * 2.0 - 1.0;
        normal = normalize(v_TBN * normal);;
    }
    else
    {
        normal = normalize(v_Normal);
    }



    float roughness = texture(u_Material.roughness, v_TextCoord).r;
    float mettalic = texture(u_Material.mettalic, v_TextCoord).r;
    float AO = texture(u_Material.ao, v_TextCoord).r;


    f_Position_Rougness =  vec4(v_FragPos, roughness);

    f_Albedo_Metallic.rgb = texture(u_Material.albedo, v_TextCoord).rgb * vec3(v_Color);
    f_Albedo_Metallic.a = mettalic;

    f_Normals = vec4(normal,1.0f);

    f_EmissionAO = vec4(texture(u_Material.emission, v_TextCoord).rgb, AO);

}
