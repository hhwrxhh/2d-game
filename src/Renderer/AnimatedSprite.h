#pragma once

#include <map>
#include <vector>

#include "Sprite.h"
#include "Texture.h"

namespace RenderEngine 
{
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(std::shared_ptr<Texture> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
			   std::string initialSubTexture);

		void insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTextureDuration);
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation) const override;

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