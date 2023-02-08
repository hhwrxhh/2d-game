#include "AiComponent.h"

#include "GameObjects/Tank.h"

AiComponent::AiComponent(Tank* pParentTank)
	: m_pParentTank(pParentTank)
{

}

void AiComponent::update(const double delta)
{
	m_pParentTank->fire();
}
