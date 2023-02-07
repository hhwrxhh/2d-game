#pragma once

#include <array>
#include <memory>
#include <glm/vec2.hpp>


class Tank;
class Level;
class StartScreen;

class Game
{
public:
	Game(const glm::ivec2& WinSize);
	~Game();

	void render();
	void update(const double delta);
	void setKey(const int key,const int action);
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
	std::shared_ptr<Tank> m_pTank;
	std::shared_ptr<Level> m_pLevel;

	std::shared_ptr<StartScreen> m_pStartScreen;
};
