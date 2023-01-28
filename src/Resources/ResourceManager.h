#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace RenderEngine 
{
	class ShaderProgram;
	class Texture;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager
{
public:
	static void setExecutablePath(const std::string& executablePath);
	static void unloadAllResources();

	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(const std::string& shaderName, 
																const std::string& vertexPath,
																const std::string& fragmentPath);
	static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(const std::string& shaderName);

	static std::shared_ptr<RenderEngine::Texture> loadTexture(const std::string& textureName,
														  const std::string& texturePath);
	static std::shared_ptr<RenderEngine::Texture> getTexture(const std::string& textureName);
	static std::shared_ptr<RenderEngine::Texture> loadTextureAtlas(std::string textureName,
															   std::string texturePath,
															   std::vector<std::string> subTextures,
															   const unsigned int subTextureWidth,
															   const unsigned int subTextureHeight);


	static std::shared_ptr<RenderEngine::Sprite> loadSprite(const std::string& spriteName,
														const std::string& shaderName,
														const std::string& textureName,
														const std::string& subTextureName = "default");
	static std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& spriteName);

	static std::shared_ptr<RenderEngine::AnimatedSprite> loadAnimatedSprite(const std::string& spriteName,
																		const std::string& shaderName,
																		const std::string& textureName,
																		const std::string& subTextureName = "default");
	static std::shared_ptr<RenderEngine::AnimatedSprite> getAnimatedSprite(const std::string& spriteName);

	static bool loadJSONResources(const std::string& JSONPath);

	static std::vector<std::vector<std::string>>& getLevels() { return m_levels; }

private:
	static std::string getFileStr(const std::string& relativeFilePath);
	 
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture>> TexturesMap;
	static TexturesMap m_textures;

	// static
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> SpritesMap;
	static SpritesMap m_sprites;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animatedSprites;

	static std::string m_path;

	static std::vector<std::vector<std::string>> m_levels;
};