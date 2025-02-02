#pragma once


#include <glm/vec3.hpp>
#include <vector>
#include <Transform/Constants.h>


namespace core
{
    struct Light {
        glm::vec3 color = { 1.0f, 1.0f, 1.0f };
        float intensity = 1.0f;
    };

    struct DirectionalLight : Light {
        glm::vec3 position = core::g_WorldUp;
        glm::vec3 target = { 0.0f, 0.0f, 0.0f };
    };

    struct SceneLights {
        Light ambientLight;
        std::vector<DirectionalLight> directionalLights;
    };

}
