#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Zombies\Bullet.h>
#include <JCEngine\AudioEngine.h>

class Bullet;

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, int bulletDamage, JCEngine::SoundEffect fireEffect);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet*>& bullets, float deltaTime);
	bool canFire(float deltaTime);

private:
	std::string _name;
	int _fireRate;
	float _spread;
	int _bulletsPerShot;
	float _bulletSpeed;
	int _bulletDamage;
	float _frameCount;

	JCEngine::SoundEffect _fireEffect;

	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet*>& bullets);
};

