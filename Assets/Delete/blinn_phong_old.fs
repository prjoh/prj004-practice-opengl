#version 410 core

#define RECIPROCAL_PI 0.3183098861837907
#define MAX_DIR_LIGHTS 4

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct DirectionalLight {
    vec3 position;
    vec3 direction;
    vec3 color;
};

uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
//uniform vec3 emissive_color;  // TODO
uniform float shininess;
uniform float opacity;

uniform bool texture_albedo_set;
uniform sampler2D texture_albedo;
// TODO: normal maps
uniform bool texture_opacity_set;
uniform sampler2D texture_opacity;
uniform bool texture_emission_set;
uniform sampler2D texture_emission;

// TODO: fog

uniform int directional_lights_size;
uniform DirectionalLight directional_lights[MAX_DIR_LIGHTS];

// TODO: Add this as option!!!
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space


void main()
{
    //vec3 diffuse_map = texture_albedo_set ? texture(texture_albedo, fs_in.TexCoords).rgb : vec3(1.0);
    //vec4 opacity_map = texture_opacity_set ? texture(texture_opacity, fs_in.TexCoords) : vec4(1.0);
    //// TODO: Add bloom https://learnopengl.com/Advanced-Lighting/Bloom
    //vec3 emission_map = texture_emission_set ? texture(texture_emission, fs_in.TexCoords).rgb : vec3(0.0);

    vec3 ambient = ambient_color;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 normal = normalize(fs_in.Normal);
    vec3 view_dir = normalize(-fs_in.FragPos);

    for (int i = 0; i < directional_lights_size; ++i) {
        DirectionalLight light = directional_lights[i];
        //vec3 light_dir = normalize(light.direction);
        vec3 light_dir = normalize(fs_in.FragPos - light.position);

        float dotNL = dot(normal, light_dir);
        float intensity = max(0.0, dotNL);
        diffuse += diffuse_color * intensity * light.color;

        vec3 halfway_dir = normalize(light_dir + view_dir);
        float dotNH = dot(normal, halfway_dir);
        intensity = pow(max(0.0, dotNH), shininess);
        specular += specular_color * intensity * light.color;
    }

    //if (texture_opacity_set)
    //{
    //    // TODO: We need to fix opacity rendering. Every mesh needs to be rendered by the renderer system (also particles). We need to sort every mesh with opacity by distance before rendering
    //    //FragColor = vec4(ambient + diffuse + specular, 1.0) * vec4(mix(diffuse_map.rgb, opacity_map.rgb, opacity_map.a), opacity_map.a);

    //    // TODO: THIS NEEDS TO BE ADDED AGAIN!!!
    //    //FragColor = vec4(ambient + diffuse + specular, 1.0) * vec4(mix(diffuse_map.rgb, opacity_map.rgb, opacity_map.a), 0.4) + emssion_map;  // TODO: What is 0.4 doing here??? Is it a hack?
    //    vec3 colorLinear = (ambient + diffuse + specular) * diffuse_map + emission_map;
    //    vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0 / screenGamma));
    //    FragColor = vec4(colorGammaCorrected, 1.0);
    //}
    //else
    //{
    vec3 colorLinear = (ambient + diffuse + specular);// * diffuse_map + emission_map;
    // apply gamma correction (assume ambientColor, diffuseColor and specColor
    // have been linearized, i.e. have no gamma correction in them)
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0 / screenGamma));
    FragColor = vec4(colorLinear, 1.0);
    //}
}
