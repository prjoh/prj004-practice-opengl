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
		Line(glm::vec3 from, glm::vec3 to, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	private:
		std::vector<float> m_vertices;
		GLuint m_vao;
		core::LineMaterial* m_material;
	};
}
