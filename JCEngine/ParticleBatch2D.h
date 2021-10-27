#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace JCEngine {
	class Particle2D {
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float lifetime = 0.0f;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position = particle.position + particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunction = defaultParticleUpdate);
		~ParticleBatch2D();

		void addParticle(const glm::vec2& position, ColorRGBA8& color, const glm::vec2& velocity, float width);

		void update(float deltaTime);
		void draw(SpriteBatch* batch);

	private:
		int findFreeParticle();

		float _decayRate = 0.1f;
		Particle2D* _particles = nullptr;
		int _maxParticles = 0;
		int _lastFreeParticle = 0;
		GLTexture _texture;

		std::function<void(Particle2D&, float)> _updateFunction;
	};
}


