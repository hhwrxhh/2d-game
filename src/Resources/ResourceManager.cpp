﻿#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"



ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileStr(const std::string& relativeFilePath) const 
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

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
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

	std::shared_ptr<Renderer::ShaderProgram>& newShader =  m_shaderPrograms.emplace(shaderName, std::make_shared <Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	
	std::cerr << "Error::ResourceManager::loadShaders ->  Can not load shader program:\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName) 
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	
	std::cerr << "Error::ResourceManager::getShaderProgram -> Can not find shader " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<Renderer::Texture> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) 
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

	std::shared_ptr<Renderer::Texture> newTexture = m_textures.emplace(textureName,
		std::make_shared<Renderer::Texture>(width, height, pixels, channels,
											GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<Renderer::Texture> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Error::ResourceManager::getTexture -> Can not find texture " << textureName << std::endl;

	return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight)
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

	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(textureName,
		std::make_shared<Renderer::Sprite>(pTexture, pShader, 
										   glm::vec2(0.f, 0.f), 
										   glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}

	std::cerr << "Error::ResourceManager::getSprite -> Can not find texture " << spriteName << std::endl;

	return nullptr;
}





