#pragma once

#include <memory>
#include <array>
#include <glm/vec2.hpp>

#include "../GameObjects/IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../System/Timer.h"

namespace RenderEngine
{
	class  Sprite;
}
class Bullet : public IGameObject
{
public:
	enum class EOrientation
	{
		Top,
		Bottom,
		Left,
		Right
	};

	Bullet(const double maxVelocity,
		   const glm::vec2& position, 
		   const glm::vec2& size,
		   const glm::vec2& explosionSize,
		   const float layer);

	virtual void render() const override;
	bool isActive() const { return m_isActive; }
	void update (const double delta) override;
	void fire (const glm::vec2& position, const glm::vec2& direction);

private:
	EOrientation m_eOrientation;
	double m_maxVelocity;
	bool m_isActive;

	glm::vec2 m_explosionSize;
	glm::vec2 m_explosionOffset;

	std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;

	std::shared_ptr<RenderEngine::Sprite> m_pSprite_explosion;
	RenderEngine::SpriteAnimator m_spriteAnimator_explosion;
	Timer m_explosionTimer;
	bool m_isExplosion;
};