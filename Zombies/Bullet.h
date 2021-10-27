#pragma once

#include <glm/glm.hpp>
#include <JCEngine/SpriteBatch.h>
#include <string>
#include <vector>
#include <Zombies\Actor.h>

class Actor;

class Bullet
{
public:
	Bullet(float speed, glm::vec2 direction, glm::vec2 position, int _lifetime, GLuint textureId, int damage);
	~Bullet();

	void draw(JCEngine::SpriteBatch& spriteBatch);
	glm::vec2 update(float deltaTime);

	bool collideWithActor(Actor* actor);

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool isDead() { return _lifetime <= 0; };
	int getDamage() { return _damage; };
	glm::vec2 getPosition() { return _position; };


private:
	float _speed;
	int _lifetime;
	glm::vec2 _direction;
	glm::vec2 _position;
	GLuint _textureId;
	int _damage;

	bool collideWithTile(glm::vec2 tilePosition);
};