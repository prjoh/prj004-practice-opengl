#version 410 core

#define MAX_DIR_LIGHTS 4
#define MAX_FRESNEL_POWER 5.0

out vec4 fs_out;

in VS_OUT {
  vec3 vertPos;
  vec3 normal;
  vec2 texCoord;
} fs_in;

struct DirectionalLight {
  // float constantAttenuation;  1.0 
  // float linearAttenuation;     0.0 
  // float quadraticAttenuation;  1.0
  vec3 position;
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
  vec4 diffuseColor = textureAlbedoSet ? texture(textureAlbedo, fs_in.texCoord) : vec4(1.0);

  vec3 normal = normalize(fs_in.normal);

  vec3 ambient = ambientColor.rgb;
                  // * diffuseColor.rgb;
  vec4 diffuse = vec4(0.0, 0.0, 0.0, diffuseColor.a);
  vec4 specular = vec4(0.0, 0.0, 0.0, diffuseColor.a);

  for (int i = 0; i < directionalLightsSize; ++i) {
    DirectionalLight light = directionalLights[i];

    // Diffuse Light

    vec3 lightDirection = light.position - fs_in.vertPos; // TODO: This is flipped..... WHY?
    vec3 unitLightDirection = normalize(lightDirection);

    float diffuseIntensity  = dot(normal, unitLightDirection);
    if (diffuseIntensity < 0.0) { continue; }

    vec4 lightColor = vec4(light.color, 1.0);

    vec4 diffuseTemp = vec4( 
      clamp(
        diffuseColor.rgb
          * lightColor.rgb
          * diffuseIntensity
        , 0.0
        , 1.0
      )
      , diffuseColor.a
    );

    // Specular Light

    vec3 viewDirection = normalize(-fs_in.vertPos);  // TODO: This is flipped..... WHY?
    vec3 halfwayDirection = normalize(unitLightDirection + viewDirection);

    float specularIntensity = clamp(dot(normal, halfwayDirection), 0.0, 1.0);

    // TODO: Let's first add some GUI to see the effect of this more clearly!
    //float fresnelFactor = dot(halfwayDirection, viewDirection); // Or dot(normal, eye).
    //fresnelFactor = max(fresnelFactor, 0.0);
    //fresnelFactor = 1.0 - fresnelFactor;
    //fresnelFactor = pow(fresnelFactor, MAX_FRESNEL_POWER);

    vec4 specularColorMaterial = vec4(specularColor, 1.0);
    //specularColorMaterial.rgb = mix(specularColorMaterial.rgb, vec3(1.0), clamp(fresnelFactor, 0.0, 1.0));

    vec4 specularTemp = clamp(
        specularColorMaterial
        * lightColor
        * pow(specularIntensity, shininess)
      , 0.0
      , 1.0
    );

    // float lightDistance = length(lightDirection);
    // float attenuation = 1.0 / ( light.constantAttenuation
    //                           + light.linearAttenuation
    //                           * lightDistance
    //                           + light.quadraticAttenuation
    //                           * (lightDistance * lightDistance));

    // diffuseTemp.rgb  *= attenuation;
    // specularTemp.rgb *= attenuation;

    diffuse.rgb  += diffuseTemp.rgb;
    specular.rgb += specularTemp.rgb;
  }

  vec4 emission = vec4(0.0);  // TODO

  fs_out.a = diffuseColor.a;
  fs_out.rgb = ambient.rgb + diffuse.rgb + specular.rgb + emission.rgb;
}
