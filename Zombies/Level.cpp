#include "Level.h"

#include <fstream>
#include <string>
#include <JCEngine/GameErrors.h>
#include <JCEngine/ResourceManager.h>
#include <iostream>

const float TILE_WIDTH = 64.0f;

Level::Level(const std::string& filePath) {
	std::ifstream file;

	file.open(filePath);
	if (file.fail()) {
		JCEngine::fatalError("Failed to load level from " + filePath);
	}

	std::string temp;
	file >> temp >> _numHumans;

	std::string firstLine;
	std::getline(file, firstLine);

	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	JCEngine::ColorRGBA8 tileColor { 255, 255, 255, 255 };

	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			glm::vec4 positionRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (_levelData[y][x]) {
			case 'B':
			case 'R':
				_spriteBatch.draw(positionRect, uvRect, JCEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, tileColor);
				break;
			case 'G':
				_spriteBatch.draw(positionRect, uvRect, JCEngine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, tileColor);
				break;
			case 'L':
				_spriteBatch.draw(positionRect, uvRect, JCEngine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, tileColor);
				break;
			case 'Z':
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				_levelData[y][x] = '.';
				break;
			case '@':
				_startPlayerPosition.x = x * TILE_WIDTH;
				_startPlayerPosition.y = y * TILE_WIDTH;
				_levelData[y][x] = '.';
				break;
			case '.':
				break;
			default:
				std::printf("unexpected symbol tile %c at (%d,%d)", _levelData[y][x], x, y);
				break;
			}
		}
	}

	_spriteBatch.end();
}
Level::~Level() {

}

void Level::draw() {
	_spriteBatch.renderBatch();
}