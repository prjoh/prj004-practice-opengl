#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;
uniform float opacity;

uniform bool texture_albedo_set;
uniform sampler2D texture_albedo;

void main()
{
    vec4 tex = texture_albedo_set ? texture(texture_albedo, TexCoords) : vec4(1.0);
    FragColor = vec4(color, opacity) * tex;
}