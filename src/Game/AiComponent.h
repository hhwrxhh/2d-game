#pragma once

class Tank;

class AiComponent
{
public:
	AiComponent(Tank* pParentTank);
	void update(const double delta);

private:
	Tank* m_pParentTank;
};