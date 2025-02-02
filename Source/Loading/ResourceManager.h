#pragma once


#include <string>
#include <map>
#include <glad/gl.h>
#include <Rendering/Material.h>
#include <Util/Singleton.h>

#include <ThirdParty/stb/stb_image.h>


namespace core
{
	//class ResourceManager
	//{
	//public:
	//	ResourceManager() = default;

	//	void loadShaders();
	//	GLuint getShader(eMaterialType materialType);
	//private:
	//	std::map<eMaterialType, GLuint> m_loadedShaders;

	//	GLuint loadShader(std::string vertexShaderPath, std::string fragmentShaderPath);
	//};


    class ResourceManager : public util::Singleton<ResourceManager> {
        friend class Singleton<ResourceManager>;
    public:
        //void RegisterResource(const std::string& key, Resource* resource) {
        //    resources[key] = resource;
        //}

        //Resource* GetResource(const std::string& key) {
        //    if (resources.find(key) != resources.end()) {
        //        return resources[key];
        //    }
        //    return nullptr;
        //}
        void loadShaders();
        GLuint getShader(types::eMaterialType materialType) const;  // TODO: That stores the shader already into the GPUs memory???

        void loadTextureFromFile(const std::string& path, bool gamma_correction = false);  // TODO: That stores the texture already into the GPUs memory???
        void loadTextureFromMemory(const std::string& textureName, const unsigned char* buffer, int len, bool gamma_correction = false);  // TODO: That stores the texture already into the GPUs memory???
        GLuint getTexture(const std::string& textureName) const;
    private:
        ResourceManager() = default;
        ~ResourceManager() = default;

        std::map<types::eMaterialType, GLuint> m_loadedShaders;
        std::map<const std::string, GLuint> m_loadedTextures;

        GLuint loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint createTexture(const unsigned char* data, int width, int height, int components, bool gamma_correction);
    };
}
