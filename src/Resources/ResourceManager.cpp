#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"


ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
std::string ResourceManager::m_path;
std::vector<std::vector<std::string>> ResourceManager::m_levels;
	

void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileStr(const std::string& relativeFilePath)
{
	std::ifstream f;

	f.open(m_path + '/' + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Error::ResourceManager::getFileStr ->  Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileStr(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "Error::ResourceManager::loadShaders -> No vertex shader " << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileStr(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "Error::ResourceManager::loadShaders ->  No fragment shader " << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader =  m_shaderPrograms.emplace(shaderName, std::make_shared <RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	
	std::cerr << "Error::ResourceManager::loadShaders ->  Can not load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName) 
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	
	std::cerr << "Error::ResourceManager::getShaderProgram -> Can not find shader " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) 
{
	int channels = 0; // rgb
	int width = 0, height = 0;

	/* 
	opengl starts read the image from lower left corner, instead stbi - upper left
	thats why we should on this lower flag ↓
	*/
	stbi_set_flip_vertically_on_load(true); 
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

 	if (!pixels)
	{
		std::cerr << "Error::ResourceManager::loadTexture ->  Can not load Texture " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture> newTexture = m_textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture>(width, height, pixels, channels,
											GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<RenderEngine::Texture> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Error::ResourceManager::getTexture -> Can not find texture " << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Texture> ResourceManager::loadTextureAtlas(std::string textureName,
																	 std::string texturePath,
																	 std::vector<std::string> subTextures,
																	 const unsigned int subTextureWidth,
																	 const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();

		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;
		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 lefBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth,
								  static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);

			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth,
								 static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

			pTexture->addSubTexture(std::move(currentSubTextureName), lefBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
			
		}
	}

	return pTexture;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
															  const std::string& shaderName,
															  const std::string& textureName,
															  const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Error::ResourceManager::loadSprite -> Can not find texture " << textureName 
			<< " for the sprite " <<  spriteName <<  std::endl;
	}

	auto pShader = getShaderProgram	(shaderName);
	if (!pShader)
	{
		std::cerr << "Error::ResourceManager::loadSprite -> Can not find shader " << shaderName
			<< " for the sprite " << spriteName << std::endl;
	}

	std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName,
		std::make_shared<RenderEngine::Sprite>(pTexture, 
										       pShader,
										       subTextureName)).first->second;

	return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Error::ResourceManager::getSprite -> Can not find texture " << spriteName << std::endl;

	return nullptr;
}

bool ResourceManager::loadJSONResources(const std::string& JSONPath)
{
	const std::string JSONString = getFileStr(JSONPath);

	if (JSONString.empty())
	{
		std::cerr << "Error::ResourceManager::loadJSONResources -> JSON " << JSONPath << " is empty" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "Error::ResourceManager::loadJSONResources -> JSON parser error in file " << JSONPath << " -> " <<
		rapidjson::GetParseError_En(parseResult.Code()) << "{" << parseResult.Offset() << "}" << std::endl;
		return false;
	}

	auto shadersIt = document.FindMember("shaders");
	if (shadersIt != document.MemberEnd())
	{
		for (const auto& currentShader : shadersIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_vertex = currentShader["filePath_vertex"].GetString();
			const std::string filePath_fragment = currentShader["filePath_fragment"].GetString();
			loadShaders(name, filePath_vertex, filePath_fragment);
		}
	}

	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
		{
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidht = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetUint();

			auto subTextureArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTextureArray.Size());

			for (const auto& currentSubTexture : subTextureArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}
			
			loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidht, subTextureHeight);
		}
	}

	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spritesIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTextureName = currentSprite["initialSubTexture"].GetString();

			auto pSprite = loadSprite(name, shader, textureAtlas, subTextureName);
			if (!pSprite)
			{
				continue;
			}

			auto framesIt = currentSprite.FindMember("frames");

			if (framesIt != currentSprite.MemberEnd())
			{
				const auto framesArray = framesIt->value.GetArray();
				std::vector<RenderEngine::Sprite::FrameDescription> framesDescriptions;
				framesDescriptions.reserve(framesArray.Size());

				for (const auto& currentFrame : framesArray)
				{
					const std::string subTextureStr = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();

					const auto pTextureAtlas = getTexture(textureAtlas);
					const auto pSubTexture = pTextureAtlas->getSubTexture(subTextureStr);

					framesDescriptions.emplace_back(pSubTexture.leftBottomUV, pSubTexture.rightTopUV, duration);
				}
				pSprite->insertFrames(std::move(framesDescriptions));
			}
		}
	}
	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
	{
		for (const auto& currentLevel : levelsIt->value.GetArray())
		{

			auto description = currentLevel["description"].GetArray();
			std::vector<std::string> levelRows;
			levelRows.reserve(description.Size());
			size_t maxLength = 0;

			for (const auto& currentRow : description)
			{
				levelRows.emplace_back(currentRow.GetString());
				if (maxLength < levelRows.back().length())
				{
					maxLength = levelRows.back().length();
				}
			}

			for (auto& currentRow : levelRows)
			{
				while (currentRow.length() < maxLength)
				{
					currentRow.append("D");
				}
			}
			m_levels.emplace_back(std::move(levelRows));
 		}
	}
	return true;
}


void ResourceManager::unloadAllResources()
{
	m_shaderPrograms.clear();
	m_path.clear();
	m_sprites.clear(); 
	m_textures.clear();
}
