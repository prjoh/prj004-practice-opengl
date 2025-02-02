#pragma once


#include <vector>
#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <Rendering/Material.h>


namespace core
{
	class Line
	{
	public:
		Line() = delete;
		Line(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color = { 1.0f, 1.0f, 1.0f });

		void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const core::SceneLights& sceneLights) const;
	private:
		// TODO: Add Transform

		std::vector<float> m_vertices;
		GLuint m_vao;
		GLuint m_vbo;

		core::LineMaterial m_material;
	};
}
