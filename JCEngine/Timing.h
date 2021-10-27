#pragma once

namespace JCEngine {
	class FPSLimiter {

	public:
		FPSLimiter();
		~FPSLimiter();

		void init(float maxFPS);
		void start();
		float stop();
		void setMaxFPS(float maxFPS);

	private:
		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;

		void CalculateFPS();
	};
}