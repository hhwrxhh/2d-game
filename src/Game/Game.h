#pragma once

#include <array>
#include <memory>
#include <glm/vec2.hpp>


class IGameState;

namespace RenderEngine
{
	class ShaderProgram;
}
class Game
{
public:
	Game(const glm::uvec2& WinSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key,const int action);
	void startNewLevel(const size_t level);
	void setWindowSize(const glm::uvec2& windowSize);
	void updateViewport();
	bool init();
	unsigned int getCurrentLevelWidth() const;
	unsigned int getCurrentLevelHeight() const;


private:
	std::array<bool, 349> m_keys;
	enum class EGameState
	{
		StartScreen,
		LevelStart,
		Level,
		Pause,
		Scores,
		GameOver
	};

	glm::ivec2 m_WinSize;
	EGameState m_eCurrentGameState;

	std::shared_ptr<IGameState> m_pCurrentGameState;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pSpriteShaderProgram;
};
