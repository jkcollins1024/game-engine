#include "Actor.h"

#include "Level.h"
#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL/SDL.h>
#include <JCEngine\ResourceManager.h>

Actor::Actor() {
}

Actor::~Actor() {

}

void Actor::draw(JCEngine::SpriteBatch& spriteBatch) {
	glm::vec4 positionRect(_position.x, _position.y, _size.x, _size.y);
	spriteBatch.draw(positionRect, _uv, _texture.id, 0, _color, _directionFacing);
}

bool Actor::applyDamage(int damage) {
	_health -= damage;
	
	return _health <= 0;
}

bool Actor::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;
	bool didCollide = false;

	//check four corners
	glm::vec2 cornerPosition1 = glm::vec2(floor(_position.x / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition2 = glm::vec2(floor((_position.x + _size.x) / 64.0f), floor(_position.y / 64.0f));
	glm::vec2 cornerPosition3 = glm::vec2(floor(_position.x / 64.0f), floor((_position.y + _size.y) / 64.0f));
	glm::vec2 cornerPosition4 = glm::vec2(floor((_position.x + _size.x) / 64.0f), floor((_position.y + _size.y) / 64.0f));

	if (cornerPosition1.x < 0 || cornerPosition4.x >= levelData[0].size() ||
		cornerPosition1.y < 0 || cornerPosition4.y >= levelData.size()) {
		return false;
	}

	if (levelData[cornerPosition1.y][cornerPosition1.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition1.x * 64.0f + 32.0f, cornerPosition1.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition1 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}
		
	if (levelData[cornerPosition2.y][cornerPosition2.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition2.x * 64.0f + 32.0f, cornerPosition2.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition2 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}
		
	if (levelData[cornerPosition3.y][cornerPosition3.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition3.x * 64.0f + 32.0f, cornerPosition3.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition3 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}
		
	if (levelData[cornerPosition4.y][cornerPosition4.x] != '.') {
		//glm::vec2 cornerPosition = glm::vec2(cornerPosition4.x * 64.0f + 32.0f, cornerPosition4.y * 64.0f + 32.0f);
		glm::vec2 cornerPosition = (cornerPosition4 * 64.0f) + glm::vec2(32.0f);
		collideTilePositions.push_back(cornerPosition);
		didCollide = true;
	}

	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
		
	return didCollide;
}

void Actor::collideWithTile(glm::vec2 tilePosition) {
	const float MIN_DISTANCE = 64.0f;

	glm::vec2 centerPlayerPosition = _position + glm::vec2(32.0f);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;

	float xdepth = MIN_DISTANCE - abs(distVec.x);
	float ydepth = MIN_DISTANCE - abs(distVec.y);

	if (xdepth > 0 || ydepth > 0) {
		if (xdepth < ydepth) {
			if (distVec.x > 0)
				_position.x += xdepth;
			else
				_position.x -= xdepth;
		}
		else {
			if (distVec.y > 0)
				_position.y += ydepth;
			else
				_position.y -= ydepth;
		}
	}
}

bool Actor::collideWithActor(Actor* actor) {
	const float MIN_DISTANCE = 64.0f;

	glm::vec2 currentCenter = _position + glm::vec2(32.0f);
	glm::vec2 otherCenter = actor->getPosition() + glm::vec2(32.0f);

	glm::vec2 distVec = currentCenter - otherCenter;

	float depth = glm::length(distVec);

	glm::vec2 collisionDepthVec = glm::normalize(distVec);	

	if (depth == 0) {
		collisionDepthVec = glm::normalize(glm::vec2(1.0f, 1.0f));
	}

	if (depth < MIN_DISTANCE) {
		collisionDepthVec = collisionDepthVec * (MIN_DISTANCE - depth);
		actor->setPosition(actor->getPosition() - (collisionDepthVec / 2.0f));
		setPosition(_position + (collisionDepthVec / 2.0f));
		return true;		
	}
	return false;
}

Player::Player(glm::vec2 position, JCEngine::Camera2D* camera) {
	_type = ActorType::PLAYER;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now
	_currentGunIndex = -1;
	_isShooting = false;

	_texture = JCEngine::ResourceManager::getTexture("Textures/player.png");

	JCEngine::GLTexture handgunWalkTexture = JCEngine::ResourceManager::getTexture("Textures/SoldierHandgunWalkAnimation.png");
	JCEngine::GLTexture shotgunWalkTexture = JCEngine::ResourceManager::getTexture("Textures/SoldierShotgunWalkAnimation.png");
	JCEngine::GLTexture handgunShootTexture = JCEngine::ResourceManager::getTexture("Textures/SoldierHandgunShootAnimation.png");
	JCEngine::GLTexture shotgunShootTexture = JCEngine::ResourceManager::getTexture("Textures/SoldierShotgunShootAnimation.png");

	m_handgunWalkTileSheet.init(handgunWalkTexture, glm::ivec2(20, 1));
	m_shotgunWalkTileSheet.init(shotgunWalkTexture, glm::ivec2(20, 1));
	m_handgunShootTileSheet.init(handgunShootTexture, glm::ivec2(3, 1));
	m_shotgunShootTileSheet.init(shotgunShootTexture, glm::ivec2(3, 1));

	_camera = camera;

	//player fastest
	_speed = 5.0f;

	//player is green
	_color = JCEngine::ColorRGBA8();
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	//always initialize player to middle of the screen, since that's where we want player to stay
	_position = position;
	_size = glm::vec2(64.0f, 64.0f);
	_health = 100; //meaningless right now, we don't take damage
}

Player::~Player() {

}

void Player::draw(JCEngine::SpriteBatch& spriteBatch) {
	float animationSpeed = 0.2f;
	glm::vec4 uvRect;
	int tileIndex, numTiles;
	glm::vec4 positionRect(_position.x, _position.y, _size.x, _size.y);
	GLuint textureID;

	m_animTime += animationSpeed;
	

	//get animation frame
	if (getCurrentGunIndex() == 0) {
		if (_isShooting) {
			numTiles = 3;

			tileIndex = (int)m_animTime % numTiles;

			if (m_animTime > numTiles) {
				_isShooting = false;
				m_animTime = 0.0f;
			}

			textureID = m_handgunShootTileSheet.texture.id;
			uvRect = m_handgunShootTileSheet.getUV(tileIndex);
		}
		else {
			numTiles = 20;

			tileIndex = (int)m_animTime % numTiles;

			if (m_animTime > numTiles) {
				_isShooting = false;
				m_animTime = 0.0f;
			}

			textureID = m_handgunWalkTileSheet.texture.id;
			uvRect = m_handgunWalkTileSheet.getUV(tileIndex);
		}
	}
	else {
		if (_isShooting) {
			numTiles = 3;

			tileIndex = (int)m_animTime % numTiles;

			if (m_animTime > numTiles) {
				_isShooting = false;
				m_animTime = 0.0f;
			}

			textureID = m_shotgunShootTileSheet.texture.id;
			uvRect = m_shotgunShootTileSheet.getUV(tileIndex);
		}
		else {
			numTiles = 20;

			tileIndex = (int)m_animTime % numTiles;

			if (m_animTime > numTiles) {
				_isShooting = false;
				m_animTime = 0.0f;
			}

			textureID = m_shotgunWalkTileSheet.texture.id;
			uvRect = m_shotgunWalkTileSheet.getUV(tileIndex);
		}
	}

	spriteBatch.draw(positionRect, uvRect, textureID, 0.0f, _color, _directionFacing);
}

void Player::move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {
	//normalize just in case it isn't already
	_direction = glm::normalize(_direction);

	setPosition(_position + (_direction * _speed * deltaTime));

	collideWithLevel(levelData);

	//return _position + (direction * _speed);
}

void Player::addGun(Gun* gun) {
	_guns.push_back(gun);
	if (_currentGunIndex == -1)
		_currentGunIndex = 0;
}

void Player::trySwitchGun(int index) {
	if (index < _guns.size()) {
		_currentGunIndex = index;
	}
}

void Player::updateGun(JCEngine::InputManager* inputManager, std::vector<Bullet*>& bullets, float deltaTime) {
	glm::vec2 mouseCoords = inputManager->getMouseCoords();
	mouseCoords = _camera->ConvertScreenToWorld(mouseCoords);

	glm::vec2 gunPosition = getPosition() + glm::vec2(32.0f) + glm::rotate(_directionFacing, -0.4f) * 32.2f;

	glm::vec2 playerPosition = getPosition() + glm::vec2(48.0f);//+ glm::rotate(glm::vec2(48.0f), _directionFacing);
	glm::vec2 bulletDirection = mouseCoords - gunPosition;
	bulletDirection = glm::normalize(bulletDirection);

	_isShooting = inputManager->isKeyDown(SDL_BUTTON_LEFT);

	_guns[_currentGunIndex]->update(_isShooting, bulletDirection, gunPosition, bullets, deltaTime);
}

void Player::setDirectionFacing(JCEngine::InputManager* inputManager) {
	glm::vec2 mouseCoords = inputManager->getMouseCoords();
	mouseCoords = _camera->ConvertScreenToWorld(mouseCoords);

	glm::vec2 playerPosition = getPosition() + glm::vec2(32.0f);
	_directionFacing = glm::normalize(mouseCoords - playerPosition);
}

Zombie::Zombie(glm::vec2 position, glm::vec2 size) {
	_position = position;
	_size = size;
	_type = ActorType::ZOMBIE;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/zombie.png");

	JCEngine::GLTexture walkTexture = JCEngine::ResourceManager::getTexture("Textures/ZombieWalkAnimation.png");

	m_walkTileSheet.init(walkTexture, glm::ivec2(4, 1));

	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);

	//get a random direction for movement
	//_direction = glm::vec2(randomDirection(randomEngine), randomDirection(randomEngine));
	//_direction = glm::normalize(_direction);

	//zombies are red
	_color = JCEngine::ColorRGBA8();
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	//zombies faster than humans, but slower than player
	_speed = 3.0f;
	_frames = 0;
	_health = 60;
}

Zombie::~Zombie() {
	
}

void Zombie::draw(JCEngine::SpriteBatch& spriteBatch) {
	float animationSpeed = 0.2f;
	glm::vec4 uvRect;
	int tileIndex, numTiles = 4;
	glm::vec4 positionRect(_position.x, _position.y, _size.x, _size.y);
	GLuint textureID;

	m_animTime += animationSpeed;

	//get animation frame
	numTiles = 4;

	tileIndex = (int)m_animTime % numTiles;

	if (m_animTime > numTiles) {
		m_animTime = 0.0f;
	}

	textureID = m_walkTileSheet.texture.id;
	uvRect = m_walkTileSheet.getUV(tileIndex);

	spriteBatch.draw(positionRect, uvRect, textureID, 0.0f, _color, _directionFacing);
}

void Zombie::move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {
	Actor* nearestHuman = getNearestHuman(actors, player);

	if (nearestHuman != nullptr) {
		_direction = glm::normalize(nearestHuman->getPosition() - _position);
		_directionFacing = _direction;

		setPosition(_position + (_direction * _speed * deltaTime));
	}
	

	collideWithLevel(levelData);
}

Actor* Zombie::getNearestHuman(std::vector<Actor*>& actors, Player* player) {
	Actor* closestHuman = nullptr;
	float smallestDistance = 9999999999.0f;

	glm::vec2 distVec = player->getPosition() - _position;
	float distance = glm::length(distVec);
	if (distance < smallestDistance) {
		closestHuman = player;
		smallestDistance = distance;
	}

	for (int i = 0; i < actors.size(); i++) {
		if (actors[i]->isHuman()) {
			glm::vec2 distVec = actors[i]->getPosition() - _position;
			float distance = glm::length(distVec);
			if (distance < smallestDistance) {
				closestHuman = actors[i];
				smallestDistance = distance;
			}
		}
	}

	return closestHuman;
}

Human::Human(glm::vec2 position, glm::vec2 size) {
	_position = position;
	_size = size;
	_type = ActorType::HUMAN;
	_uv = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //uv should just be standard for now

	_texture = JCEngine::ResourceManager::getTexture("Textures/human.png");

	JCEngine::GLTexture walkTexture = JCEngine::ResourceManager::getTexture("Textures/HumanWalkAnimation.png");

	m_walkTileSheet.init(walkTexture, glm::ivec2(12, 1));

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);

	//get a random direction for movement
	_direction = glm::vec2(randomDirection(randomEngine), randomDirection(randomEngine));
	_direction = glm::normalize(_direction);
	_directionFacing = _direction;

	//humans are blue
	_color = JCEngine::ColorRGBA8();
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	//humans slower than zombies
	_speed = 2.0f;
	_frames = 0;
	_health = 20;
}

Human::~Human() {

}

void Human::draw(JCEngine::SpriteBatch& spriteBatch) {
	float animationSpeed = 0.2f;
	glm::vec4 uvRect;
	int tileIndex, numTiles = 4;
	glm::vec4 positionRect(_position.x, _position.y, _size.x, _size.y);
	GLuint textureID;

	m_animTime += animationSpeed;

	//get animation frame
	numTiles = 12;
	tileIndex = (int)m_animTime % numTiles;

	if (m_animTime > numTiles) {
		m_animTime = 0.0f;
	}

	textureID = m_walkTileSheet.texture.id;
	uvRect = m_walkTileSheet.getUV(tileIndex);

	spriteBatch.draw(positionRect, uvRect, textureID, 0.0f, _color, glm::rotate(_directionFacing, -0.5f * (float)M_PI));
}

void Human::move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {
	//normalize just in case it isn't already
	_direction = glm::normalize(_direction);

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randomRotation(-0.5f, 0.5f);

	if (_frames == 20) {
		_direction = glm::rotate(_direction, randomRotation(randomEngine));
		_directionFacing = _direction;
		_frames = 0;
	}
	else {
		_frames++;
	}

	_position += _direction * _speed * deltaTime;

	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randomRotation(randomEngine));
		_directionFacing = _direction;
	}
		
}