#include "IGameObject.h"

IGameObject::IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: m_position(position), m_size(size), m_rotation(rotation), m_layer(layer), m_direction(0.f, 1.f), m_velocity(0),
	  m_eObjectType(objectType)
{
}

IGameObject::~IGameObject()
{
}

void IGameObject::setVelocity(const double velocity)
{
	m_velocity = velocity;
}
