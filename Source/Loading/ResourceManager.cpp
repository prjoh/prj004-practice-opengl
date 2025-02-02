#define STB_IMAGE_IMPLEMENTATION
#include <Loading/ResourceManager.h>


#include <algorithm>
#include <fstream>
#include <sstream>

#include <Application/Application.h>


#include <iostream>


void core::ResourceManager::loadShaders()
{
    std::string basePath = app::Application::s_Path;
    std::string shaderName;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    // Get the starting and ending values of the enum
    int startValue = static_cast<int>(types::eMaterialType::First) + 1;
    int endValue = static_cast<int>(types::eMaterialType::Last);

    for (int i = startValue; i < endValue; ++i)
    {
        shaderName = core::types::s_MaterialTypeString[i];

        std::transform(shaderName.begin(), shaderName.end(), shaderName.begin(),
            [](unsigned char c) { return std::tolower(c); });

        SDL_Log("Material -> %s", shaderName.c_str());

        vertexShaderPath = basePath + "Assets\\Shaders\\" + shaderName + "\\" + shaderName + ".vs";
        fragmentShaderPath = basePath + "Assets\\Shaders\\" + shaderName + "\\" + shaderName + ".fs";

        SDL_Log("## %s", vertexShaderPath.c_str());
        SDL_Log("## %s", fragmentShaderPath.c_str());

        GLuint shaderID = loadShader(vertexShaderPath, fragmentShaderPath);  // TODO: Error checking

        SDL_Log("Shader compiled: %s | %d", shaderName.c_str(), shaderID);

        types::eMaterialType type = static_cast<types::eMaterialType>(i);
        m_loadedShaders.emplace(type, shaderID);
    }
}

// TODO: ERROR HANDLING
GLuint core::ResourceManager::getShader(types::eMaterialType materialType) const
{
    auto it = m_loadedShaders.find(materialType);
    if (it == m_loadedShaders.end()) {
        return -1;
    }

    return it->second;
}

// TODO: ERROR HANDLING
void core::ResourceManager::loadTextureFromFile(const std::string& path, bool gamma_correction)
{
    int width, height, components;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 0);

    if (data == NULL)
    {
        // TODO: HANDLE ERROR
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture failed to load at path: %s\n", path.c_str());
        return;
    }

    GLuint texture_id = createTexture(data, width, height, components, gamma_correction);
    const std::string textureName = path.substr(path.find_last_of("/\\") + 1);
    m_loadedTextures.emplace(textureName, texture_id);
    std::cout << textureName << " loaded!" << std::endl;

    stbi_image_free(data);
}

// TODO: ERROR HANDLING
void core::ResourceManager::loadTextureFromMemory(const std::string& textureName, const unsigned char* buffer, int len, bool gamma_correction)
{
    int width, height, components;
    unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &components, 0);

    if (data == NULL)
    {
        // TODO: HANDLE ERROR
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture with name %s failed to load from buffer.", textureName);
        return;
    }

    GLuint texture_id = createTexture(data, width, height, components, gamma_correction);
    m_loadedTextures.emplace(textureName, texture_id);

    stbi_image_free(data);
}

// TODO: ERROR HANDLING
GLuint core::ResourceManager::getTexture(const std::string& textureName) const
{
    auto it = m_loadedTextures.find(textureName);
    if (it == m_loadedTextures.end()) {
        return -1;
    }

    return it->second;
}

// TODO: Make this a loadFile method
// TODO: Error handling
GLuint core::ResourceManager::loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    // Load from file
    std::string vshCodeStr;
    std::string fshCodeStr;
    std::ifstream vshFile;
    std::ifstream fshFile;
    // ensure ifstream objects can throw exceptions:
    vshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vshFile.open(vertexShaderPath);
        fshFile.open(fragmentShaderPath);
        std::stringstream vshStream, fshStream;
        // read file's buffer contents into streams
        vshStream << vshFile.rdbuf();
        fshStream << fshFile.rdbuf();
        // close file handlers
        vshFile.close();
        fshFile.close();
        // convert stream into string
        vshCodeStr = vshStream.str();
        fshCodeStr = fshStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: %s;%s\n%s", vertexShaderPath.c_str(), fragmentShaderPath.c_str(), e.what());
        return -1;
    }

    const char* vshCode = vshCodeStr.c_str();
    const char* fshCode = fshCodeStr.c_str();

    // Compile vertex shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshCode, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    // Compile fragment shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fshCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
        return -1;
    }

    // Create shader program
    GLuint id;
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return id;
}

GLuint core::ResourceManager::createTexture(const unsigned char* data, int width, int height, int components, bool gamma_correction)
{
    GLint internal_format;
    GLint format;
    if (components == 1)
        internal_format = GL_RED;
    else if (components == 3)
    {
        internal_format = gamma_correction ? GL_SRGB : GL_RGB;
        format = GL_RGB;
    }
    else if (components == 4)
    {
        internal_format = gamma_correction ? GL_SRGB_ALPHA : GL_RGBA;
        format = GL_RGBA;
    }
    else
    {
        // TODO: Handle error!!!
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture_id;
}
