#pragma once

#include <map>
#include <vector>

#include "Sprite.h"
#include "Texture.h"

namespace Renderer 
{
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
			   std::string initialSubTexture, glm::vec2& position = glm::vec2(0.f), 
			   glm::vec2& size = glm::vec2(1.f), const float rotation = 0.f);

		void insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration);
		void render() const override;

		void setState(const std::string& newState);
		void update(uint64_t delta);

	private:
		size_t m_currentFrame = 0;
		uint64_t m_currentAnimationTime = 0;
		mutable bool m_dirty = false;

		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMap;
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrentAnimationDurations;
	};
} 