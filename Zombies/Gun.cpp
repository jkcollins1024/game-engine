#include "Gun.h"
#include <JCEngine\ResourceManager.h>
#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, int bulletDamage, JCEngine::SoundEffect fireEffect) {
	_name = name;
	_fireRate = fireRate;
	_bulletsPerShot = bulletsPerShot;
	_spread = spread;
	_bulletSpeed = bulletSpeed;
	_bulletDamage = bulletDamage;
	_frameCount = fireRate;
	_fireEffect = fireEffect;
}

Gun::~Gun() {

}

void Gun::update(bool isMouseDown, const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet*>& bullets, float deltaTime) {
	_frameCount += 1.0f * deltaTime;

	if (_frameCount >= _fireRate && isMouseDown) {
		fire(direction, position, bullets);
		_frameCount = 0;
	}

}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet*>& bullets) {

	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randomRotation(-_spread, _spread);

	for (int i = 0; i < _bulletsPerShot; i++) {
		Bullet* bullet = new Bullet(_bulletSpeed, glm::rotate(direction, randomRotation(randomEngine)), position, 150, JCEngine::ResourceManager::getTexture("Textures/circle.png").id, _bulletDamage);

		bullets.push_back(bullet);
		_fireEffect.play();
	}
}