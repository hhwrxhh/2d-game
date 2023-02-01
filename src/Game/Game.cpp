﻿#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"

#include "../Resources/ResourceManager.h"

#include "GameObjects/Tank.h"
#include "Level.h"

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
    if (m_pTank)
    {
        m_pTank->render();
    }
    if (m_pLevel)
    {
        m_pLevel->render();
    }   
}

void Game::update(const uint64_t delta)
{
    if (m_pLevel)
    {
        m_pLevel->update(delta);
    }
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

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);
    m_WinSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_WinSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_WinSize.x), 0.f, static_cast<float>(m_WinSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    m_pTank = std::make_unique<Tank>(0.0000001f, m_pLevel->getPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);

    return true; 
}

size_t Game::getCurrentLevelWidth() const
{
    return m_pLevel->getLevelWidth();
}

size_t Game::getCurrentLevelHeight() const
{
    return m_pLevel->getLevelHeight();
}
