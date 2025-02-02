#include <Rendering/Material.h>

#include <Loading/ResourceManager.h>


core::Material::Material(core::types::eMaterialType type)
	: m_type(type)
{
	m_shader = core::ResourceManager::getInstance().getShader(type);  // TODO: ERROR HANDLING!!!
}

void core::Material::use() const
{
	glUseProgram(m_shader);
}

void core::Material::setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
	uniformSetMat4f("model", model);
	uniformSetMat4f("view", view);
	uniformSetMat4f("projection", projection);
}

void core::Material::uniformSetInt(const char* name, const int data) const {
	glUniform1i(glGetUniformLocation(m_shader, name), data);
}

void core::Material::uniformSetFloat(const char* name, const float data) const {
	glUniform1f(glGetUniformLocation(m_shader, name), data);
}

void core::Material::uniformSetVec2fv(const char* name, const glm::vec2& data) const {
	glUniform2fv(glGetUniformLocation(m_shader, name), 1, &data[0]);
}

void core::Material::uniformSetVec2f(const char* name, const float x, const float y) const {
	glUniform2f(glGetUniformLocation(m_shader, name), x, y);
}

void core::Material::uniformSetVec3fv(const char* name, const glm::vec3& data) const {
	glUniform3fv(glGetUniformLocation(m_shader, name), 1, &data[0]);
}

void core::Material::uniformSetVec3f(const char* name, const float x, const float y, const float z) const {
	glUniform3f(glGetUniformLocation(m_shader, name), x, y, z);
}

void core::Material::uniformSetVec4fv(const char* name, const glm::vec4& data) const {
	glUniform4fv(glGetUniformLocation(m_shader, name), 1, &data[0]);
}

void core::Material::uniformSetVec4f(const char* name, const float x, const float y, const float z, const float w) const {
	glUniform4f(glGetUniformLocation(m_shader, name), x, y, z, w);
}

void core::Material::uniformSetMat2f(const char* name, const glm::mat2& data) const {
	glUniformMatrix2fv(glGetUniformLocation(m_shader, name), 1, GL_FALSE, &data[0][0]);
}

void core::Material::uniformSetMat3f(const char* name, const glm::mat3& data) const {
	glUniformMatrix3fv(glGetUniformLocation(m_shader, name), 1, GL_FALSE, &data[0][0]);
}

void core::Material::uniformSetMat4f(const char* name, const glm::mat4& data)  const {
	glUniformMatrix4fv(glGetUniformLocation(m_shader, name), 1, GL_FALSE, &data[0][0]);
}


core::LineMaterial::LineMaterial(const glm::vec3& color)
	: Material(core::types::eMaterialType::Line), m_color(color)
{
}

void core::LineMaterial::setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const
{
	uniformSetVec3fv("color", m_color);
	uniformSetFloat("opacity", m_opacity);
}




core::BasicMaterial::BasicMaterial(const glm::vec3& color, const std::string& textureName)
	: Material(core::types::eMaterialType::Basic), m_color(color)
{
	if (textureName.length() > 0)
	{
		m_textureId = core::ResourceManager::getInstance().getTexture(textureName);  // TODO: ERROR HANDLING!!!
		m_textureSet = true;  // TODO: We should make sure that texture is actaully loaded!!!
	}
}

void core::BasicMaterial::setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const
{
	uniformSetVec3fv("color", m_color);
	uniformSetFloat("opacity", m_opacity);
	uniformSetInt("texture_albedo_set", m_textureSet);

	if (m_textureSet) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
	}
}


core::BlinnPhongMaterial::BlinnPhongMaterial(const glm::vec3& color, const glm::vec3& specular, glm::vec3 emissive, float shininess, 
	const std::string& albedoTextureName, const std::string& normalTextureName, const std::string& opacityTextureName, const std::string& emissionTextureName)
	: Material(core::types::eMaterialType::Blinn_Phong), m_color(color), m_specular(specular), m_emissive(emissive), m_shininess(shininess)
{
	if (albedoTextureName.length() > 0)
	{
		m_albedoTextureId = core::ResourceManager::getInstance().getTexture(albedoTextureName);  // TODO: ERROR HANDLING!!!
		m_albedoTextureSet = true;  // TODO: We should make sure that texture is actaully loaded!!!
	}

	if (normalTextureName.length() > 0)
	{
		m_normalTextureId = core::ResourceManager::getInstance().getTexture(normalTextureName);  // TODO: ERROR HANDLING!!!
		m_normalTextureSet = true;  // TODO: We should make sure that texture is actaully loaded!!!
	}

	if (opacityTextureName.length() > 0)
	{
		m_opacityTextureId = core::ResourceManager::getInstance().getTexture(opacityTextureName);  // TODO: ERROR HANDLING!!!
		m_opacityTextureSet = true;  // TODO: We should make sure that texture is actaully loaded!!!
	}

	if (emissionTextureName.length() > 0)
	{
		m_emissionTextureId = core::ResourceManager::getInstance().getTexture(emissionTextureName);  // TODO: ERROR HANDLING!!!
		m_emissionTextureSet = true;  // TODO: We should make sure that texture is actaully loaded!!!
	}
}

void core::BlinnPhongMaterial::setUniforms(const glm::vec3& cameraPosition, const SceneLights& lights) const
{
	// TODO: Debug print ALL parameters
	uniformSetVec3fv("view_pos", cameraPosition);
	uniformSetVec3fv("ambient_color", lights.ambientLight.color * lights.ambientLight.intensity);
	uniformSetVec3fv("diffuse_color", m_color);
	uniformSetVec3fv("specular_color", m_specular);
	uniformSetFloat("shininess", m_shininess);
	uniformSetFloat("opacity", m_opacity);
	uniformSetInt("directional_lights_size", (int)lights.directionalLights.size());

	for (int i = 0; i < lights.directionalLights.size(); ++i) {
		DirectionalLight light = lights.directionalLights[i];

		glm::vec3 direction = glm::normalize(light.target - light.position);
		glm::vec3 color = light.color * light.intensity;

		std::string num = std::to_string(i);
		uniformSetVec3fv(std::string("directional_lights[" + num + "].position").c_str(), light.position);
		//uniformSetVec3fv(std::string("directional_lights[" + num + "].direction").c_str(), direction);
		uniformSetVec3fv(std::string("directional_lights[" + num + "].color").c_str(), color);
	}

	uniformSetInt("texture_albedo_set", m_albedoTextureSet);
	if (m_albedoTextureSet) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_albedoTextureId);
	}

	// TODO: Normal maps

	uniformSetInt("texture_opacity_set", m_opacityTextureSet);
	if (m_opacityTextureSet) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_opacityTextureId);
	}

	uniformSetInt("texture_emission_set", m_emissionTextureSet);
	if (m_emissionTextureSet) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_emissionTextureId);
	}
}
