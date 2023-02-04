#pragma once

#include <memory>
#include <array>
#include <glm/vec2.hpp>

#include "../GameObjects/IGameObject.h"

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
		   const float layer);

	virtual void render() const override;
	bool isActive() const { return m_isActive; }
	void fire (const glm::vec2& position, const glm::vec2& direction);
	virtual void onCollision() override;

private:
	EOrientation m_eOrientation;
	double m_maxVelocity;
	bool m_isActive;

	std::shared_ptr<RenderEngine::Sprite> m_pSprite_top;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_bottom;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;
};