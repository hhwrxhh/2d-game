#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <memory>

class IGameObject;

class Level
{
public:
	Level(const std::vector<std::string>& levelDescription);
	void render() const;
	void update(const uint64_t delta) const;

private:
	unsigned int m_width;
	unsigned int m_height;

	std::vector<std::shared_ptr<IGameObject>> m_mapObject;

};