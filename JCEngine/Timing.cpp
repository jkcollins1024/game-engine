#include "Timing.h"

#include <SDL/SDL.h>

namespace JCEngine {
	FPSLimiter::FPSLimiter() {

	}

	FPSLimiter::~FPSLimiter() {

	}

	void FPSLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::start() {
		_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::stop() {
		CalculateFPS();

		float totalTicks = SDL_GetTicks() - _startTicks;
		//limit fps to max fps
		if (1000 / _maxFPS > totalTicks) {
			SDL_Delay(1000 / _maxFPS - totalTicks);
		}

		return _fps;
	}

	void FPSLimiter::setMaxFPS(float maxFPS) {
		_maxFPS = maxFPS;
	}

	void FPSLimiter::CalculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();
		float currTicks;

		currTicks = SDL_GetTicks();
		_frameTime = currTicks - prevTicks;

		prevTicks = currTicks;

		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		int count;
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame + 1;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}

		frameTimeAverage = frameTimeAverage / count;

		if (frameTimeAverage > 0) {
			_fps = 1000 / frameTimeAverage;
		}
		else {
			frameTimeAverage = 0.0f;
		}

		currentFrame++;
	}
}