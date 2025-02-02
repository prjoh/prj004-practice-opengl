#include <Rendering/Camera.h>

#include <Application/Input/Input.h>
#include <glm/gtx/quaternion.hpp>
#include <Transform/Constants.h>
#include <glm/matrix.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>


core::Camera::Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up, float aspect, float fov, float zNear, float zFar)
	: m_aspect(aspect), m_fov(fov), m_zNear(zNear), m_zFar(zFar)
{
	m_transform.position(position);

	auto front = glm::normalize(center - position);
	m_transform.orientation(glm::quatLookAt(front, glm::normalize(up)));

	//auto orientation = glm::quatLookAt(front, glm::normalize(up));
	//orientation.x *= -1.0f;  // TODO: Why??
	//m_transform.orientation(orientation);
}

void core::Camera::move(float elapsedTimeS)
{
	const float sensitivity = 0.1f;

	auto& input = app::Input::getInstance();

	float pitch = (float)input.mouseDeltaY() * sensitivity;
	float yaw = (float)input.mouseDeltaX() * sensitivity;
	glm::quat q_pitch = glm::angleAxis(glm::radians(pitch), core::g_WorldRight);
	glm::quat q_yaw = glm::angleAxis(glm::radians(yaw), core::g_WorldUp);

	m_transform.orientation(glm::normalize(q_pitch * m_transform.orientation() * q_yaw));

	float velocity = 5.0f * elapsedTimeS;
	
	if (input.keyPressed(SDLK_w))
		m_transform.position(m_transform.position() + m_transform.front() * velocity);
	if (input.keyPressed(SDLK_s))
		m_transform.position(m_transform.position() - m_transform.front() * velocity);
	if (input.keyPressed(SDLK_a))
		m_transform.position(m_transform.position() - m_transform.right() * velocity);
	if (input.keyPressed(SDLK_d))
		m_transform.position(m_transform.position() + m_transform.right() * velocity);
}
