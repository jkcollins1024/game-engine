#pragma once

#include <string>
#include <vector>

#include <JCEngine/SpriteBatch.h>

class Level
{
public:
	//load the level
	Level(const std::string& filePath);
	~Level();

	void draw();

	const std::vector<std::string>& getLevelData() const { return _levelData; };

	//getters
	glm::vec2 getStartPlayerPosition() const { return _startPlayerPosition; };
	const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; };
	int getNumHumans() const { return _numHumans; };
	int getWidth() const { return _levelData[0].size(); };
	int getHeight() const { return _levelData.size(); };

private:
	std::vector<std::string> _levelData;
	int _numHumans;
	JCEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPosition;
	std::vector<glm::vec2> _zombieStartPositions;
};

