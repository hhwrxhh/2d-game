#pragma once 

#include <glm/vec2.hpp>

#include "../../Physics/PhysicsEngine.h"

class IGameObject
{
public:
	enum class EObjectType
	{
		BetonWall,
		Border,
		BrickWall,
		Bullet,
		Eagle,
		Ice,
		Tank,
		Tree,
		Water,
		Unknown
	};
	IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual ~IGameObject();

	virtual void render() const = 0;	
	virtual void update(double delta) { };
	virtual glm::vec2& getCurrentPosition() { return m_position; }
	virtual glm::vec2& getCurrentDirection() { return m_direction; }
	virtual double getCurrentVelocity() { return m_velocity; }
	virtual void setVelocity(const double velocity);

	virtual const glm::vec2& getSize() const { return m_size; }
	virtual const std::vector<Physics::AABB>& getColliders() const { return m_colliders; }

	virtual EObjectType getObjectType() const { return m_eObjectType; }
	virtual bool colliders(const EObjectType objectType) { return true; }
	virtual void onCollision() {};

protected:
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;

	glm::vec2 m_direction;
	double m_velocity;

	std::vector<Physics::AABB> m_colliders;
	EObjectType m_eObjectType;
};