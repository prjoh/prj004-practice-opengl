#pragma once

#include <Geometry/Geometry.h>
#include <Rendering/Material.h>
#include <Rendering/SceneLights.h>
#include <Transform/Transform.h>


namespace core
{
	class Mesh
	{
	public:
		Mesh() = default;
	
		void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const core::SceneLights& sceneLights) const;

		// TODO: Remove this
		core::Geometry* m_geometry;
		core::Material* m_material;
		core::Transform m_transform;
	private:
		//core::Geometry* m_geometry;
		//core::Material* m_material;
	};
}
