#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

#include "IGameState.h"

class IGameObject;
class Tank;

class Level : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;

	Level(const std::vector<std::string>& levelDescription);
	virtual void render() const;
	virtual void update(const double delta);
	size_t getLevelWidth() const;
	size_t getLevelHeight() const;
	virtual void processInput(std::array<bool, 349>& keys);

	const glm::ivec2& getPlayerRespawn_1() const { return m_playerRespawn_1; }
	const glm::ivec2& getPlayerRespawn_2() const { return m_playerRespawn_2; }
	const glm::ivec2& getEnemyRespawn_1() const { return m_enemyRespawn_1; }
	const glm::ivec2& getEnemyRespawn_2() const { return m_enemyRespawn_2; }
	const glm::ivec2& getEnemyRespawn_3() const { return m_enemyRespawn_3; }

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

	std::vector<std::shared_ptr<IGameObject>> getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight);
	void initPhysics();
 
private:
	unsigned int m_widthBlocks;
	unsigned int m_heightBlocks;
	unsigned int m_widthPixels = 0;
	unsigned int m_heightPixels = 0;

	glm::ivec2 m_playerRespawn_1;
	glm::ivec2 m_playerRespawn_2;
	glm::ivec2 m_enemyRespawn_1;
	glm::ivec2 m_enemyRespawn_2;
	glm::ivec2 m_enemyRespawn_3;

	std::vector<std::shared_ptr<IGameObject>> m_mapObject;
	std::shared_ptr<Tank> m_pTank;
};