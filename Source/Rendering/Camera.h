#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Transform/Transform.h>


namespace core
{
    class Camera
    {
    public:
        Camera() = delete;
        Camera(const glm::vec3& position, 
               const glm::vec3& center = { 0.0f, 0.0f, 0.0f },
               const glm::vec3& up = core::g_WorldUp,
               float aspect = 800.0f / 600.0f,
               float fov = 45.0f,
               float zNear = 0.01f,
               float zFar = 1000.0f);
 
        const glm::vec3 position() { return m_transform.position(); };
        const glm::mat4 viewMatrix() { return glm::mat4_cast(m_transform.orientation()) * glm::translate(glm::mat4(1.0f), -m_transform.position()); };
        const glm::mat4 projectionMatrix() { return glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar); };

        void move(float elapsedTimeS);

        Transform m_transform;
    private:
        //bool inverted_pitch = true;

        float m_fov;
        float m_aspect;
        float m_zNear;
        float m_zFar;
    };
}
