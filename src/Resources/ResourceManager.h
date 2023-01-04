#pragma once
#include <string>
#include <memory>
#include <map>

namespace Renderer 
{
	class ShaderProgram;
	class Texture;
	class Sprite;
}

class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, 
														const std::string& vertexPath,
														const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);

	std::shared_ptr<Renderer::Texture> loadTexture(const std::string& textureName, 
												   const std::string& texturePath);
	std::shared_ptr<Renderer::Texture> getTexture(const std::string& textureName);


	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, 
												const std::string& textureName,
												const std::string& shaderName,
												const unsigned int spriteWidth,
												const unsigned int spriteHeight);
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);
	 


private:
	std::string getFileStr(const std::string& relativeFilePath) const;
	 
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture>> TexturesMap;
	TexturesMap m_textures;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	std::string m_path;

};