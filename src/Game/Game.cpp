#include "Game.h"

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
#include "../Renderer/Renderer.h"

#include <GLFW/glfw3.h>

Game::Game(const glm::uvec2& WinSize)
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
    m_pCurrentGameState->render();
}

void Game::update(const double delta)
{
    switch (m_eCurrentGameState)
    {
    case EGameState::StartScreen:
        if (m_keys[GLFW_KEY_ENTER])
        {
            m_eCurrentGameState = EGameState::Level;
            startNewLevel(0);
        }
        break;
    case EGameState::Level:
        m_pCurrentGameState->processInput(m_keys);
        m_pCurrentGameState->update(delta);
        break;
    }
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

void Game::startNewLevel(const size_t level)
{
    auto pLevel = std::make_shared<Level>(ResourceManager::getLevels()[level]);
    m_pCurrentGameState = pLevel;
    Physics::PhysicsEngine::setCurrentLevel(pLevel);
    updateViewport();
}

void Game::setWindowSize(const glm::uvec2& windowSize)
{
    m_WinSize = windowSize;
    updateViewport();
}

void Game::updateViewport()
{
    const float map_aspect_ratio = static_cast<float>(getCurrentLevelWidth()) / getCurrentLevelHeight();

    unsigned int viewPortWidth = m_WinSize.x;
    unsigned int viewPortHeight = m_WinSize.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortRightOffset = 0;

    if (static_cast<float>(m_WinSize.x) / m_WinSize.y > map_aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(m_WinSize.y * map_aspect_ratio);
        viewPortLeftOffset = (m_WinSize.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(m_WinSize.x / map_aspect_ratio);
        viewPortRightOffset = (m_WinSize.y - viewPortHeight) / 2;
    }
    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortRightOffset);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(getCurrentLevelWidth()), 0.f, static_cast<float>(getCurrentLevelHeight()), -100.f, 100.f);
    m_pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    m_pSpriteShaderProgram = ResourceManager::getShaderProgram("SpriteShader");
    if (!m_pSpriteShaderProgram)
    {
        std::cerr << "Error::Game::init ->Can not find shader program " << "SpriteShader" << std::endl;
        return false;
    }

    m_pSpriteShaderProgram->use();
    m_pSpriteShaderProgram->setInt("tex", 0);

    m_pCurrentGameState = std::make_shared<StartScreen>(ResourceManager::getStartScreen());
    setWindowSize(m_WinSize);

    return true; 
}

unsigned int Game::getCurrentLevelWidth() const
{
    return m_pCurrentGameState->getStateWidth();
}

unsigned int Game::getCurrentLevelHeight() const
{
    return m_pCurrentGameState->getStateHeight();
}
