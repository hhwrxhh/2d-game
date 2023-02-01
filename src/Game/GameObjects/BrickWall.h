#pragma once

#include <memory>
#include <array>

#include "IGameObject.h"

namespace RenderEngine
{
	class Sprite;
}
class BrickWall : public IGameObject
{
public:

	enum class EBrickWallType
	{
		All,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	enum class EBrickState {
		All = 0,
		TopLeft,
		TopRight,
		Top,
		BottomLeft,
		Left,
		TopRight_BottomLeft,
		Top_BottomLeft,
		BottomRight,
		TopLeft_BottomRight,
		Right,
		Top_BottomRight,
		TopLeft_Bottom,
		TopRight_Bottom,
		Bottom,
		Destroyed
	};

	enum class EBrickLocation
	{
		TopLeft,
		TopRight,
		BottomLeft, 
		BottomRight
	};

	BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void render() const override;
	virtual void update(double delta) override;

private:
	void renderBrick(const EBrickLocation eBrickLocation) const;

	std::array<glm::vec2, 4> m_blockOffesets;
	std::array<std::shared_ptr<RenderEngine::Sprite>, 15> m_sprites;
	std::array<EBrickState, 4> m_eCurrentBrickState;


};