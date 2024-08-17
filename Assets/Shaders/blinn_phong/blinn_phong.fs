#version 410 core

#define MAX_DIR_LIGHTS 4

out vec4 fs_out;

out VS_OUT {
  vec3 vertPos;
  vec3 normal;
  vec2 texCoord;
} fs_in;

struct DirectionalLight {
  // float constantAttenuation;  1.0 
  // float linearAttenuation;     0.0 
  // float quadraticAttenuation;  1.0
  vec3 position;
  vec3 direction;
  vec3 color;
};

// Material properties  // TODO: Make a struct?
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
//uniform vec3 emissiveColor;  // TODO
uniform float shininess;
uniform float opacity;

uniform bool textureAlbedoSet;
uniform sampler2D textureAlbedo;
// TODO: normal map
uniform bool textureOpacitySet;
uniform sampler2D textureOpacity;
uniform bool textureEmissionSet;
uniform sampler2D textureEmission;

// TODO: fog

uniform int directionalLightsSize;
uniform DirectionalLight directionalLights[MAX_DIR_LIGHTS];

void main()
{
  vec4 diffuseColor = textureAlbedoSet ? texture(textureAlbedo, fs_in.texCoord) : vec4(0.0, 0.0, 0.0, 1.0);

  vec3 normal = normalize(fs_in.normal);

  vec3 ambient = ambientColor.rgb;
                  // * diffuseColor.rgb;
  vec4 diffuse = vec4(0.0, 0.0, 0.0, diffuseColor.a);
  vec4 specular = vec4(0.0, 0.0, 0.0, diffuseColor.a);

  for (int i = 0; i < directionalLightsSize; ++i) {
    DirectionalLight light = directionalLights[i];

    vec3 lightDirection = light.direction.xyz;

    vec3 unitLightDirection = normalize(lightDirection);
    vec3 viewDirection       = normalize(-fs_in.vertPos.xyz);
    vec3 reflectedDirection = normalize(-reflect(unitLightDirection, normal));
    // vec3 halfwayDirection   = normalize(unitLightDirection + viewDirection);

    // float lightDistance = length(lightDirection);

    // float attenuation = 1.0 / ( light.constantAttenuation
    //                           + light.linearAttenuation
    //                           * lightDistance
    //                           + light.quadraticAttenuation
    //                           * (lightDistance * lightDistance));

    float diffuseIntensity  = dot(normal, unitLightDirection);
    if (diffuseIntensity < 0.0) { continue; }

    vec4 lightDiffuseColor = vec4(light.color, 1.0);

    vec4 diffuseTemp = vec4( 
      clamp(
        diffuseColor.rgb
          * lightDiffuseColor.rgb
          * diffuseIntensity
        , 0.0
        , 1.0
      )
      , diffuseColor.a
    );

    float specularIntensity = max(dot(reflectedDirection, viewDirection), 0.0);

    vec4 specularTemp  = clamp(
      vec4(specularColor, 1.0)
        * lightDiffuseColor
        * pow(specularIntensity, shininess)
      , 0.0
      , 1.0
    );

    // diffuseTemp.rgb  *= attenuation;
    // specularTemp.rgb *= attenuation;

    diffuse.rgb  += diffuseTemp.rgb;
    specular.rgb += specularTemp.rgb;
  }

  vec3 emission = vec4(0.0);  // TODO

  fs_out.a = diffuseColor.a
  fs_out = ambient.rgb + diffuse.rgb + specular.rgb + emission.rgb;
}
