#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace Renderer 
{
	class ShaderProgram;
	class Texture;
	class Sprite;
	class AnimatedSprite;
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
	std::shared_ptr<Renderer::Texture> loadTextureAtlas(std::string textureName,
														std::string texturePath,
														std::vector<std::string> subTextures,
														const unsigned int subTextureWidth,
														const unsigned int subTextureHeight);


	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName, 
												const std::string& shaderName,
												const std::string& textureName,
												const unsigned int spriteWidth,
												const unsigned int spriteHeight,
												const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

	std::shared_ptr<Renderer::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
		const std::string& shaderName,
		const std::string& textureName,
		const unsigned int spriteWidth,
		const unsigned int spriteHeight,
		const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

private:
	std::string getFileStr(const std::string& relativeFilePath) const;
	 
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture>> TexturesMap;
	TexturesMap m_textures;

	// static
	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	AnimatedSpritesMap m_animatedSprites;

	std::string m_path;

};