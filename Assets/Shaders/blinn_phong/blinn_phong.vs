#version 410 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
// TODO: layout (location = 3) in vec3 binormal;
// TODO: layout (location = 4) in vec3 tangent;

out VS_OUT {
  vec3 vertPos;
  vec3 normal;
  vec2 texCoord;
} vs_out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
// uniform mat4 normalMatrix;

void main()
{
  vs_out.vertPos = modelViewMatrix * vertex;
  vs_out.normal = transpose(inverse(modelViewMatrix)) * normal;
  vs_out.texCoord = texCoord;

  gl_Position = projectionMatrix * vs_out.vertPos;
}
