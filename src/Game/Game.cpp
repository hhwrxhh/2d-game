#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
#include "../Resources/ResourceManager.h"
#include "Tank.h"
#include <GLFW/glfw3.h>


Game::Game(const glm::ivec2& WinSize)
	: m_eCurrentGameState(EGameState::Active),
      m_WinSize(WinSize)
{
	m_keys.fill(false);
}

Game::~Game()
{
}

void Game::render()
{
    //ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if (m_pTank)
    {
        m_pTank->render();
    }
}

void Game::update(const uint64_t delta)
{
    // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
    // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);

    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true); 
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("SpriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Error::Game::init ->Can not find shader program " << "SpriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Error::Game::init ->Can not find texture atlas" << "mapTextureAtlas" << std::endl;
        return false;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Error::Game::init ->Can not find tank atlas" << "tanksTextureAtlas" << std::endl;
        return false;
    }

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "SpriteShader", "mapTextureAtlas", 100, 100, "bottomBeton");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000)); // one frame - one second
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000)); // one frame - one second
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000)); // one frame - one second

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000)); // one frame - one second
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000)); // one frame - one second

    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));
    pAnimatedSprite->setState("waterState");

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_WinSize.x), 0.f, static_cast<float>(m_WinSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pTanksAnimatedSprite = ResourceManager::getAnimatedSprite("tanksAnimatedSprite");
    if (!pTanksAnimatedSprite)
    {       
        std::cerr << "Error::Game::init ->Can not find animated sprite" << "tanksAnimatedSprite" << std::endl;
        return false;
    }
    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));

    return true;
}
