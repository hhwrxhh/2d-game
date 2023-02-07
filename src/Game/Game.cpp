﻿#include "Game.h"

#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Sprite.h"

#include "../Resources/ResourceManager.h"

#include "GameObjects/Tank.h"

#include "../Physics/PhysicsEngine.h"

#include <GLFW/glfw3.h>

Game::Game(const glm::ivec2& WinSize)
    : m_WinSize(WinSize)
    , m_eCurrentGameState(EGameState::StartScreen) 
{
	m_keys.fill(false);
}

Game::~Game()
{
}

void Game::render()
{
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        m_pStartScreen->render();
        break;
    case EGameState::Level:
        if (m_pTank)
        {
            m_pTank->render();
        }
        if (m_pLevel)
        {
            m_pLevel->render();
        }
        break;
    }
}

void Game::update(const double delta)
{
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        if (m_keys[GLFW_KEY_ENTER])
        {
            m_eCurrentGameState = EGameState::Level;
        }
        break;
    case EGameState::Level:
        if (m_pLevel)
        {
            m_pLevel->update(delta);
        }
        if (m_pTank)
        {
            if (m_keys[GLFW_KEY_W])
            {
                m_pTank->setOrientation(Tank::EOrientation::Top);
                m_pTank->setVelocity(m_pTank->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_A])
            {
                m_pTank->setOrientation(Tank::EOrientation::Left);
                m_pTank->setVelocity(m_pTank->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_S])
            {
                m_pTank->setOrientation(Tank::EOrientation::Bottom);
                m_pTank->setVelocity(m_pTank->getMaxVelocity());
            }
            else if (m_keys[GLFW_KEY_D])
            {
                m_pTank->setOrientation(Tank::EOrientation::Right);
                m_pTank->setVelocity(m_pTank->getMaxVelocity());
            }
            else
            {
                m_pTank->setVelocity(0);
            }

            if (m_pTank && m_keys[GLFW_KEY_SPACE])
            {
                m_pTank->fire();
            }
            m_pTank->update(delta);
        }
        break;
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

    m_pStartScreen = std::make_shared<StartScreen>(ResourceManager::getStartScreen());

    m_pLevel = std::make_shared<Level>(ResourceManager::getLevels()[1]);
    m_WinSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_WinSize.y = static_cast<int>(m_pLevel->getLevelHeight());
    Physics::PhysicsEngine::setCurrentLevel(m_pLevel);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_WinSize.x), 0.f, static_cast<float>(m_WinSize.y), -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    m_pTank = std::make_shared<Tank>(0.05, m_pLevel->getPlayerRespawn_2(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);
    Physics::PhysicsEngine::addDynamicGameObject(m_pTank);

    return true; 
}

unsigned int Game::getCurrentLevelWidth() const
{
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        return m_pStartScreen->getStateWidth();
    case EGameState::Level:
        return m_pLevel->getStateWidth();
    }
}

unsigned int Game::getCurrentLevelHeight() const
{
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        return m_pStartScreen->getStateHeight();
    case EGameState::Level:
        return m_pLevel->getStateHeight();
    }
}
