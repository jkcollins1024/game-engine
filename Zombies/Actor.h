#pragma once

#include <glm/glm.hpp>
#include <JCEngine/SpriteBatch.h>
#include <JCEngine/GLTexture.h>
#include <JCEngine/InputManager.h>
#include <Zombies\Gun.h>
#include <JCEngine\Camera2D.h>
#include <JCEngine/TileSheet.h>

enum class ActorType { ZOMBIE, HUMAN, PLAYER };

class Zombie;
class Human;
class Player;
class Gun;
class Bullet;

class Actor
{
public:
	Actor();
	~Actor();

	virtual void draw(JCEngine::SpriteBatch& spriteBatch);

	//depends on actors around it - will need some processing in main game
	virtual void move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) {};

	//returns true if dead
	bool applyDamage(int damage);

	bool collideWithLevel(const std::vector<std::string>& levelData);
	bool collideWithActor(Actor* actor);
	bool isZombie() { return _type == ActorType::ZOMBIE; };
	bool isPlayer() { return _type == ActorType::PLAYER; };
	bool isHuman() { return _type == ActorType::HUMAN; };

	//getters
	glm::vec2 getPosition() { return _position; };

	//setters
	void setPosition(glm::vec2 position) { _position = position; };
	void setDirection(glm::vec2 direction) { _direction = direction; _directionFacing = direction; };

protected:
	float _speed = 20;
	float m_animTime = 0.0f;
	glm::vec2 _position;
	glm::vec2 _size;
	glm::vec4 _uv;
	JCEngine::ColorRGBA8 _color;
	JCEngine::GLTexture _texture;
	JCEngine::TileSheet m_walkTileSheet;
	ActorType _type;
	glm::vec2 _direction;
	glm::vec2 _directionFacing;
	int _frames;
	int _health;

	void collideWithTile(glm::vec2 tilePosition);
};

class Player : public Actor {
public:
	Player(){}
	Player(glm::vec2 position, JCEngine::Camera2D* camera);
	~Player();

	virtual void draw(JCEngine::SpriteBatch& spriteBatch) override;
	virtual void move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) override;

	void addGun(Gun* gun);
	void trySwitchGun(int index);
	void updateGun(JCEngine::InputManager* inputManager, std::vector<Bullet*>& bullets, float deltaTime);

	void setDirectionFacing(JCEngine::InputManager* inputManager);

	//getters
	int getCurrentGunIndex() { return _currentGunIndex; };

private:
	std::vector<Gun*> _guns;
	int _currentGunIndex;
	bool _isShooting;

	JCEngine::Camera2D* _camera;
	JCEngine::TileSheet m_handgunWalkTileSheet;
	JCEngine::TileSheet m_shotgunWalkTileSheet;
	JCEngine::TileSheet m_handgunShootTileSheet;
	JCEngine::TileSheet m_shotgunShootTileSheet;
};

class Zombie : public Actor {
public:
	Zombie(glm::vec2 position, glm::vec2 size);
	~Zombie();

	Actor* getNearestHuman(std::vector<Actor*>& actors, Player* player);

	virtual void draw(JCEngine::SpriteBatch& spriteBatch) override;
	virtual void move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) override;
};

class Human : public Actor {
public:
	Human(glm::vec2 position, glm::vec2 size);
	~Human();
	
	virtual void draw(JCEngine::SpriteBatch& spriteBatch) override;
	virtual void move(const std::vector<std::string>& levelData, std::vector<Actor*>& actors, Player* player, float deltaTime) override;	
};

