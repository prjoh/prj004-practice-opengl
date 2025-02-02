#pragma once


#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include <string>

#include <Rendering/SceneLights.h>


#define stringify( name ) #name


namespace core::types
{
	enum eMaterialType {
		First,  // Do NOT delete!
		Line,
		Basic,
		Blinn_Phong,
		Last,  // Do NOT delete!
	};
	static const char* s_MaterialTypeString[] =
	{
	  stringify(First),  // Do NOT delete!
	  stringify(Line),
	  stringify(Basic),
	  stringify(Blinn_Phong),
	  stringify(Last),  // Do NOT delete!
	};
}

namespace core
{
	class Material
	{
	public:
		Material() = delete;
		Material(types::eMaterialType type);

		void use() const;

		void setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
		virtual void setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const = 0;

		void setTransparent(bool transparent) { m_transparent = transparent; };
		void setOpacity(float opacity) { m_opacity = opacity; };
		void setWireframeMode(bool enabled) { m_wireframe = enabled; };
	protected:
		void uniformSetInt(const char* name, const int data) const;
		void uniformSetFloat(const char* name, const float data) const;
		void uniformSetVec2fv(const char* name, const glm::vec2& data) const;
		void uniformSetVec2f(const char* name, const float x, const float y) const;
		void uniformSetVec3fv(const char* name, const glm::vec3& data) const;
		void uniformSetVec3f(const char* name, const float x, const float y, const float z) const;
		void uniformSetVec4fv(const char* name, const glm::vec4& data) const;
		void uniformSetVec4f(const char* name, const float x, const float y, const float z, const float w) const;
		void uniformSetMat2f(const char* name, const glm::mat2& data) const;
		void uniformSetMat3f(const char* name, const glm::mat3& data) const;
		void uniformSetMat4f(const char* name, const glm::mat4& data) const;

		types::eMaterialType m_type;
		GLuint m_shader;
		bool m_transparent = false;  // TODO: This is currently not considered!
		float m_opacity = 1.0f;      // TODO: This is currently not considered! What about blend modes?
		bool m_wireframe = false;  // TODO: This is currently not considered!
	};


	// TODO: Move to own file???
	class LineMaterial : public Material {
	public:
		LineMaterial() = delete;
		LineMaterial(const glm::vec3& color);

		void setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const override;
	private:
		glm::vec3 m_color;
	};


	// TODO: Move to own file???
	class BasicMaterial : public Material {
	public:
		BasicMaterial() = delete;
		BasicMaterial(const glm::vec3& color, const std::string& textureName = "");

		void setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const override;
	private:
		glm::vec3 m_color;
		GLuint m_textureId = -1;  // TODO: This is wrong
		bool m_textureSet = false;
	};


	// TODO: Move to own file???
	class BlinnPhongMaterial : public Material {
	public:
		BlinnPhongMaterial() = delete;
		BlinnPhongMaterial(const glm::vec3& color, 
						   const glm::vec3& specular, 
						   glm::vec3 emissive, 
						   float shininess,
						   const std::string& albedoTextureName = "",
						   const std::string& normalTextureName = "",
						   const std::string& opacityTextureName = "",
						   const std::string& emissionTextureName = "");

		void setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const override;
	private:
		glm::vec3 m_color;

		glm::vec3 m_specular;
		glm::vec3 m_emissive;
		float m_shininess;

		GLuint m_albedoTextureId = -1;  // TODO: This is wrong
		bool m_albedoTextureSet = false;

		GLuint m_normalTextureId = -1;  // TODO: This is wrong
		bool m_normalTextureSet = false;

		GLuint m_opacityTextureId = -1;  // TODO: This is wrong
		bool m_opacityTextureSet = false;

		GLuint m_emissionTextureId = -1;  // TODO: This is wrong
		bool m_emissionTextureSet = false;
	};
}
