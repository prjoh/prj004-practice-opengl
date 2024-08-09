#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    vec4 vertPos4 = view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(vertPos4).xyz / vertPos4.w;
    vs_out.Normal = (transpose(inverse(model)) * vec4(aNormal, 1.0)).xyz;  // TODO: Precalcualte normalMatrix
    vs_out.TexCoords = aTexCoords;

    gl_Position = projection * vertPos4;
    /*
    vec4 mv_position = view * model * vec4(aPos, 1.0);
    //vs_out.ViewPos = -mv_position.xyz;  // What is this?
    vs_out.ViewPos = mv_position.xyz;
    */

    /*
    vec4 vertPos4 = view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(vertPos4).xyz / vertPos4.w;
    vs_out.Normal = (transpose(inverse(model)) * vec4(aNormal, 1.0)).xyz;
    vs_out.TexCoords = aTexCoords;

    gl_Position = projection * vertPos4;
    */
}
