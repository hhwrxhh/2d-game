#include <iostream>

#include "AnimatedSprite.h"

namespace Renderer
{
	AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
		 std::string initialSubTexture, glm::vec2& position, glm::vec2& size, const float rotation)
		: Sprite(std::move(pTexture), std::move(pShaderProgram), std::move(initialSubTexture), position, size, rotation)
	{
		m_pCurrentAnimationDurations = m_statesMap.end();
	}

	void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration)
	{
		m_statesMap.emplace(std::move(state), std::move(subTextureDuration));
	}

	void AnimatedSprite::render() const 
	{

		if (m_dirty)
		{
			auto subTexture = m_pTexture->getSubTexture(m_pCurrentAnimationDurations->second[m_currentFrame].first);

			const GLfloat textureCoords[] =
			{
				// u  v
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
			}; 
			m_textureCoordsBuffer.update(textureCoords, 2 * 4 * sizeof(GLfloat));
			m_dirty = false;
		}

		Sprite::render();
	}

	void AnimatedSprite::setState(const std::string& newState)
	{
		auto it = m_statesMap.find(newState);
		if (it == m_statesMap.end())
		{
			std::cerr << "Error::AnimatedSprite::setState -> Can not find animation state" << std::endl;
		}

		if (m_pCurrentAnimationDurations != it)
		{
			m_currentAnimationTime = 0;
			m_currentFrame = 0;
			m_pCurrentAnimationDurations = it;
			m_dirty = true;
		}
		
	}

	void AnimatedSprite::update(uint64_t delta)
	{
		if (m_pCurrentAnimationDurations != m_statesMap.end())
		{
			m_currentAnimationTime += delta;

			while (m_currentAnimationTime >= m_pCurrentAnimationDurations->second[m_currentFrame].second)
			{
				m_currentAnimationTime -= m_pCurrentAnimationDurations->second[m_currentFrame].second;
				++m_currentFrame;
				m_dirty = true;

				if (m_currentFrame == m_pCurrentAnimationDurations->second.size())
				{
					m_currentFrame = 0;
				}
			}

			
		}
	}
	
}
