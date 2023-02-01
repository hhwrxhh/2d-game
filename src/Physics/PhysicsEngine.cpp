#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"

std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;

void PhysicsEngine::init()
{

}

void PhysicsEngine::terminate()
{
	m_dynamicObjects.clear();
}

void PhysicsEngine::update(const double delta)
{
	for (auto& currentObject : m_dynamicObjects)
	{
		if (currentObject->getCurrentVelocity() > 0)
		{
			glm::vec2 currentDirection = currentObject->getCurrentDirection();
			float currentVelocity = static_cast<float>(currentObject->getCurrentVelocity());
			float l_delta = static_cast<float>(delta);

			currentObject->getCurrentPosition() += currentDirection * currentVelocity * l_delta;
		}
	}
}

void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
{
	m_dynamicObjects.insert(std::move(pGameObject));
}
