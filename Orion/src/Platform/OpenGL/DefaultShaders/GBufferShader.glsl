#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TextureCoord;
layout(location = 6) in float a_TextureSlot;


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


layout(location = 0) out vec3 f_Position;
layout(location = 1) out vec3 f_Normal;
layout(location = 2) out vec4 f_AlbedoSpec;







struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normals;

    float shininess;
};



in vec3 v_Normal; 
in vec4 v_Color;
in vec2 v_TextCoord;
in vec3 v_FragPos;
in mat3 v_TBN;


uniform Material u_Material;



void main()
{
    if (texture(u_Material.diffuse, v_TextCoord).a < 0.1f) discard;

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
    // store the fragment position vector in the first gbuffer texture
    f_Position = v_FragPos;
    // also store the per-fragment normals into the gbuffer
    f_Normal = normal;
    // and the diffuse per-fragment color
    f_AlbedoSpec.rgb = texture(u_Material.diffuse, v_TextCoord).rgb * vec3(v_Color);
    // store specular intensity in gAlbedoSpec's alpha component
    f_AlbedoSpec.a = texture(u_Material.specular, v_TextCoord).r;

}
