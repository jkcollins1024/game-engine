#include "ParticleEngine2D.h"

namespace JCEngine {

	ParticleEngine2D::ParticleEngine2D() {

	}

	ParticleEngine2D::~ParticleEngine2D() {
		for (auto& b : _particleBatches) {
			delete b;
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* batch) {
		_particleBatches.push_back(batch);
	}

	void ParticleEngine2D::update(float deltaTime) {
		for (auto& b : _particleBatches) {
			b->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& b : _particleBatches) {
			spriteBatch->begin();
			b->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}	
	}
}