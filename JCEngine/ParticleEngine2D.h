#pragma once

#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace JCEngine {
	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		//will delete in destructor, do not deallocate elsewhere
		void addParticleBatch(ParticleBatch2D* batch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> _particleBatches;
	};
}


