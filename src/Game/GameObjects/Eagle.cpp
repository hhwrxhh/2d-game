#include "Eagle.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"

Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
    : IGameObject(position, size, rotation, layer)
    , m_eCurrentEagleState(EEagleState::Alive)
    , m_sprites{ ResourceManager::getSprite("eagle"),
                 ResourceManager::getSprite("eagle_dead") }
{
}

void Eagle::render() const
{
    m_sprites[static_cast<size_t>(m_eCurrentEagleState)]->render(m_position, m_size, m_rotation, m_layer);
}

void Eagle::update(double delta)
{
}


