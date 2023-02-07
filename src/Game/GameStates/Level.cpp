#include "Level.h"

#include <algorithm>
#include <cmath>


#include "../GameObjects/BrickWall.h"
#include "../GameObjects/BetonWall.h"
#include "../GameObjects/Tree.h"
#include "../GameObjects/Ice.h"
#include "../GameObjects/Water.h"
#include "../GameObjects/Eagle.h"
#include "../GameObjects/Border.h"
#include "../GameObjects/Tank.h"

#include <GLFW/glfw3.h>

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

	m_widthBlocks = levelDescription[0].length();
	m_heightBlocks = levelDescription.size();
	m_widthPixels = m_widthBlocks * BLOCK_SIZE;
	m_heightPixels= m_heightBlocks * BLOCK_SIZE;

	m_mapObject.reserve(m_widthBlocks * m_heightBlocks + 4);
	unsigned int currentBottomOffset = BLOCK_SIZE * (m_heightBlocks - 1) + BLOCK_SIZE / 2.f;


	// if json for respawning does not exist
	m_playerRespawn_1 = { BLOCK_SIZE * (m_widthBlocks / 2 - 1), BLOCK_SIZE / 2 };
	m_playerRespawn_2 = { BLOCK_SIZE * (m_widthBlocks / 2 + 3), BLOCK_SIZE / 2 };

	m_enemyRespawn_1 = { BLOCK_SIZE, m_heightBlocks * BLOCK_SIZE - BLOCK_SIZE / 2 };
	m_enemyRespawn_2 = { BLOCK_SIZE * (m_widthBlocks / 2 + 1), m_heightBlocks * BLOCK_SIZE - BLOCK_SIZE / 2 };
	m_enemyRespawn_3 = { BLOCK_SIZE * m_widthBlocks, m_heightBlocks * BLOCK_SIZE - BLOCK_SIZE / 2 };

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		for (const char currentElement : currentRow)
		{
			switch (currentElement)
			{
			case 'K':
				m_playerRespawn_1 = { currentLeftOffset, currentBottomOffset };
				m_mapObject.emplace_back(nullptr);
				break;
			case 'L':
				m_playerRespawn_2 = { currentLeftOffset, currentBottomOffset };
				m_mapObject.emplace_back(nullptr);
				break;
			case 'M':
				m_enemyRespawn_1 = { currentLeftOffset, currentBottomOffset };
				m_mapObject.emplace_back(nullptr);
				break;
			case 'N':
				m_enemyRespawn_2 = { currentLeftOffset, currentBottomOffset };
				m_mapObject.emplace_back(nullptr);
				break;
			case 'O':
				m_enemyRespawn_3 = { currentLeftOffset, currentBottomOffset };
				m_mapObject.emplace_back(nullptr);
				break;
			default:
				m_mapObject.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
				break;
			}

			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
	 
	// bottom border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

	// top border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_heightBlocks * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

	// left border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

	// right border
	m_mapObject.emplace_back(std::make_shared<Border>(glm::vec2((m_widthBlocks + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

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
	m_pTank->render();
}

void Level::update(const double delta)
{
	for (const auto& currentMapObject : m_mapObject)
	{
		if (currentMapObject)
		{
			currentMapObject->update(delta);
		}
	}
	m_pTank->update(delta);
}
size_t Level::getLevelWidth() const
{
	return (m_widthBlocks + 3) * BLOCK_SIZE;
}

size_t Level::getLevelHeight() const
{
	return (m_heightBlocks + 1) * BLOCK_SIZE;
}

void Level::processInput(std::array<bool, 349>& keys)
{
	if (keys[GLFW_KEY_W])
	{
		m_pTank->setOrientation(Tank::EOrientation::Top);
		m_pTank->setVelocity(m_pTank->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_A])
	{
		m_pTank->setOrientation(Tank::EOrientation::Left);
		m_pTank->setVelocity(m_pTank->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_S])
	{
		m_pTank->setOrientation(Tank::EOrientation::Bottom);
		m_pTank->setVelocity(m_pTank->getMaxVelocity());
	}
	else if (keys[GLFW_KEY_D])
	{
		m_pTank->setOrientation(Tank::EOrientation::Right);
		m_pTank->setVelocity(m_pTank->getMaxVelocity());
	}
	else
	{
		m_pTank->setVelocity(0);
	}

	if (m_pTank && keys[GLFW_KEY_SPACE])
	{
		m_pTank->fire();
	}
}

unsigned int Level::getStateWidth() const
{
	return static_cast<unsigned int>((m_widthBlocks + 3) * BLOCK_SIZE);
}

unsigned int Level::getStateHeight() const
{
	return static_cast<unsigned int>((m_heightBlocks + 1) * BLOCK_SIZE);
}

std::vector<std::shared_ptr<IGameObject>> Level::getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& topRight)
{
	std::vector<std::shared_ptr<IGameObject>> output;
	output.reserve(9);

	glm::vec2 bottomLeft_converted(std::clamp(bottomLeft.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widthPixels)),
								   std::clamp(m_heightPixels - bottomLeft.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixels)));
	glm::vec2 topRight_converted(std::clamp(topRight.x - BLOCK_SIZE, 0.f, static_cast<float>(m_widthPixels)),
								 std::clamp(m_heightPixels - topRight.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(m_heightPixels)));

	size_t startX = static_cast<size_t>(floor(bottomLeft_converted.x / BLOCK_SIZE));
	size_t endX =   static_cast<size_t>(ceil(topRight_converted.x / BLOCK_SIZE));

	size_t startY = static_cast<size_t>(floor(topRight_converted.y / BLOCK_SIZE));
	size_t endY = static_cast<size_t>(ceil(bottomLeft_converted.y / BLOCK_SIZE));
	

	/*std::cout << "startX " << startX << ", endX " << endX << std::endl;
	std::cout << "startY " << startY << ", endY " << endY << std::endl;*/
	/*std::cout << "bottomLeft_converted.x " << bottomLeft_converted.x << ", bottomLeft_converted.y " << bottomLeft_converted.y << std::endl;
	std::cout << "topRight_converted.x " << topRight_converted.x << ", topRight_converted.y " << topRight_converted.y << std::endl;*/

	for (size_t currentColumn = startX; currentColumn < endX; ++currentColumn)
	{
		for (size_t currentRow = startY; currentRow < endY; ++currentRow)
		{
			auto& currentObject = m_mapObject[currentRow * m_widthBlocks + currentColumn];
			if (currentObject)
			{
				output.push_back(currentObject);
			} 
		}
	}

	if (endX >= m_widthBlocks)
	{
		output.push_back(m_mapObject[m_mapObject.size() - 1]);
	}
	if (startX <= 1)
	{
		output.push_back(m_mapObject[m_mapObject.size() - 2]);
	}
	if (startY <= 1)
	{
		output.push_back(m_mapObject[m_mapObject.size() - 3]);
	}
	if (endY >= m_heightBlocks)
	{
		output.push_back(m_mapObject[m_mapObject.size() - 4]);
	}

	return output;
}

void Level::initPhysics()
{
	m_pTank = std::make_shared<Tank>(Tank::ETankType::Player1Yellow_type1, 0.05, getPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);
	Physics::PhysicsEngine::addDynamicGameObject(m_pTank);
}
