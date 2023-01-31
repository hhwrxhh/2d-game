#pragma once

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

#include <array>
#include <memory>

namespace RenderEngine {
    class Sprite;
}

class Eagle : public IGameObject {
public:

    enum class EEagleState
    {
        Alive = 0,
        Dead
    };

    Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void render() const override;
    virtual void update(uint64_t delta) override;

private:
    std::array<std::shared_ptr<RenderEngine::Sprite>, 2> m_sprites;
    EEagleState m_eCurrentEagleState;
};