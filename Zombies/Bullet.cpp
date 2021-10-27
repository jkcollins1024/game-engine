#include "Bullet.h"

#include <glm/glm.hpp>
#include <JCEngine/GLTexture.h>
#include <JCEngine/ResourceManager.h>

Bullet::Bullet(float speed, glm::vec2 direction, glm::vec2 position, int lifetime, GLuint textureId, int damage) {
	_speed = speed;
	_direction = direction;
	_position = position;
	_lifetime = lifetime;
	_textureId = textureId;
	_damage = damage;
}

Bullet::~Bullet() {

}

void Bullet::draw(JCEngine::SpriteBatch& spriteBatch) {
	JCEngine::ColorRGBA8 color;
	color.r = 0;
	color.b = 0;
	color.g = 0;
	color.a = 255;

	glm::vec4 positionForSprite = glm::vec4(_position.x, _position.y, 10, 10);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(positionForSprite, uv, _textureId, 0, color);
}

bool Bullet::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;

	//check four corners
	glm::vec2 cornerPosition1 = glm::vec2(floor(_position.x / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition2 = glm::vec2(floor((_position.x + 30) / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition3 = glm::vec2(floor(_position.x / 64.0f), floor((_position.y + 30) / 64.0f));
	glm::vec2 cornerPosition4 = glm::vec2(floor((_position.x + 30) / 64.0f), floor((_position.y + 30) / 64.0f));

	if (levelData[cornerPosition1.y][cornerPosition1.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition1.x * 64.0f + 32.0f, cornerPosition1.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition1 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
	}

	if (levelData[cornerPosition2.y][cornerPosition2.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition2.x * 64.0f + 32.0f, cornerPosition2.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition2 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
	}

	if (levelData[cornerPosition3.y][cornerPosition3.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition3.x * 64.0f + 32.0f, cornerPosition3.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition3 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
	}

	if (levelData[cornerPosition4.y][cornerPosition4.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition4.x * 64.0f + 32.0f, cornerPosition4.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition4 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
	}

	for (int i = 0; i < collideTilePositions.size(); i++) {
		if (collideWithTile(collideTilePositions[i]))
			return true;
	}

	return false;
}

bool Bullet::collideWithTile(glm::vec2 tilePosition) {
	const float AGENT_RADIUS = 32.0f;
	const float MIN_DISTANCE = 64.0f;

	glm::vec2 centerPlayerPosition = _position + glm::vec2(32.0f);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;

	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	if (xdepth > 0 || ydepth > 0) {
		return true;
	}
}

bool Bullet::collideWithActor(Actor* actor) {
	const float MIN_DISTANCE = 37.0f;

	glm::vec2 currentCenter = _position;
	glm::vec2 otherCenter = actor->getPosition() + glm::vec2(32.0f);

	glm::vec2 distVec = currentCenter - otherCenter;

	float depth = glm::length(distVec);

	if (depth < MIN_DISTANCE) {
		return true;
	}
	return false;
}

//returns position to determine collision when bullet should be destroyed
glm::vec2 Bullet::update(float deltaTime) {
	_position += _direction * _speed * deltaTime;
	_lifetime--;

	return _position;
}