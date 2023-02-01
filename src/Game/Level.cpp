#include "Level.h"

#include "../Resources/ResourceManager.h"

#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWall.h"
#include "GameObjects/Tree.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"


std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
{
	switch (description) 
	{
	case '0':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.f);
	case '1':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.f);
	case '2':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.f);
	case '3':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.f);
	case '4':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.f);
	case '5':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::TopRight, position, size, rotation, 0.f);
	case '6':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Bottom, position, size, rotation, 0.f);
	case '7':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Left, position, size, rotation, 0.f);
	case '8':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Top, position, size, rotation, 0.f);
	case '9':
		return std::make_shared<BetonWall>(BetonWall::EBetonWallType::All, position, size, rotation, 0.f);
	case 'A':
		return std::make_shared<Water>(position, size, rotation, 0.f);
	case 'B':
		return std::make_shared<Tree>(position, size, rotation, 1.f);
	case 'C':
		return std::make_shared<Ice>(position, size, rotation, -1.f);
	case 'E':
		return std::make_shared<Eagle>(position, size, rotation, 0.f);
	case 'G':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.f);
	case 'H':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.f);
	case 'I':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.f);
	case 'J':
		return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.f);

	case 'D':
		return nullptr;
	default:
		std::cerr << "Error::Level::createGameObjectFromDescription -> Failed to find GameObject description " << description << std::endl;
	}

	return nullptr;
}
Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cerr << "Error::Level::Level ->  levelDescription is empty" << std::endl;
	}

	m_width = levelDescription[0].length();
	m_height = levelDescription.size();

	m_mapObject.reserve(m_width * m_height + 4);
	unsigned int currentBottomOffset = BLOCK_SIZE * (m_height - 1) + BLOCK_SIZE / 2.f;


	// if json for respawning does not exist
	m_playerRespawn_1 = { BLOCK_SIZE * (m_width / 2 - 1), BLOCK_SIZE / 2 };
	m_playerRespawn_2 = { BLOCK_SIZE * (m_width / 2 + 3), BLOCK_SIZE / 2 };

	m_enemyRespawn_1 = { BLOCK_SIZE, m_height * BLOCK_SIZE - BLOCK_SIZE / 2 };
	m_enemyRespawn_2 = { BLOCK_SIZE * (m_width / 2 + 1), m_height * BLOCK_SIZE - BLOCK_SIZE / 2 };
	m_enemyRespawn_3 = { BLOCK_SIZE * m_width, m_height * BLOCK_SIZE - BLOCK_SIZE / 2 };

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		for (const char currentElement : currentRow)
		{
			switch (currentElement)
			{
			case 'K':
				m_playerRespawn_1 = { currentLeftOffset, currentBottomOffset };
				break;
			case 'L':
				m_playerRespawn_2 = { currentLeftOffset, currentBottomOffset };
				break;
			case 'M':
				m_enemyRespawn_1 = { currentLeftOffset, currentBottomOffset };
				break;
			case 'N':
				m_enemyRespawn_2 = { currentLeftOffset, currentBottomOffset };
				break;
			case 'O':
				m_enemyRespawn_3 = { currentLeftOffset, currentBottomOffset };
				break;
			default:
				m_mapObject.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
				break;
			}

			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
	 
	// botton border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));
	// top border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_height * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));
	// left border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_height + 1) * BLOCK_SIZE), 0.f, 0.f));
	// right border 
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2((m_width + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (m_height + 1) * BLOCK_SIZE), 0.f, 0.f));


}

void Level::render() const
{
	for (const auto& currentMapObject : m_mapObject)
	{
		if (currentMapObject)
		{
			currentMapObject->render();
		}
	}
}

void Level::update(const uint64_t delta) const
{
	for (const auto& currentMapObject : m_mapObject)
	{
		if (currentMapObject)
		{
			currentMapObject->update(delta);
		}
	}
}

size_t Level::getLevelWidth() const
{
	return (m_width + 3) * BLOCK_SIZE;
}

size_t Level::getLevelHeight() const
{
	return (m_height + 1) * BLOCK_SIZE;
}

