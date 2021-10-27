#include "ParticleBatch2D.h"

namespace JCEngine {

	ParticleBatch2D::ParticleBatch2D(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunction) {
		_maxParticles = maxParticles;
		_decayRate = decayRate;
		_texture = texture;
		_updateFunction = updateFunction;

		_particles = new Particle2D[maxParticles];
	}

	ParticleBatch2D::~ParticleBatch2D() {
		delete[] _particles;
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, ColorRGBA8& color, const glm::vec2& velocity, float width) {
		int index = findFreeParticle();

		auto& p = _particles[index];

		p.position = position;
		p.color = color;
		p.velocity = velocity;
		p.width = width;
		p.lifetime = 1.0f;
	}

	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < _maxParticles; i++) {
			if (_particles[i].lifetime > 0) {
				_updateFunction(_particles[i], deltaTime);//_particles[i].update(deltaTime);
				_particles[i].lifetime -= _decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* batch) {
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < _maxParticles; i++) {
			auto& p = _particles[i];
			if (p.lifetime > 0) {
				glm::vec4 destRect = glm::vec4(p.position.x, p.position.y, p.width, p.width);
				batch->draw(destRect, uvRect, _texture.id, 0, p.color);
			}
		}
	}

	int ParticleBatch2D::findFreeParticle() {
		for (int i = _lastFreeParticle; i < _maxParticles; i++) {
			if (_particles[i].lifetime <= 0.0f) {
				_lastFreeParticle = i + 1;
				return i;
			}
		}

		for (int i = 0; i < _lastFreeParticle; i++) {
			if (_particles[i].lifetime <= 0.0f) {
				_lastFreeParticle = i + 1;
				return i;
			}
		}

		//no particles are free, so overwrite first
		return 0;
	}
}
