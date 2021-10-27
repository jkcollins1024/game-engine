#include "AudioEngine.h"
#include "GameErrors.h"

namespace JCEngine {
	void SoundEffect::play(int loops) {
		if (Mix_PlayChannel(-1, _chunk, loops) == -1) {
			if (Mix_PlayChannel(0, _chunk, loops) == -1) {
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
			
	}

	void Music::play(int loops) {
		if (Mix_PlayMusic(_music, loops))
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
	}

	void Music::pause() {
		Mix_PauseMusic();
	}

	void Music::stop() {
		Mix_HaltMusic();
	}

	void Music::resume() {
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine() {

	}
	AudioEngine::~AudioEngine() {

	}

	void AudioEngine::init() {
		//paramter combination of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));

		_isInitialized = true;
	}

	void AudioEngine::destroy() {
		if (_isInitialized) {
			_isInitialized = false;

			for (auto& it : _soundEffectCache) {
				Mix_FreeChunk(it.second);
			}
			for (auto& it : _musicCache) {
				Mix_FreeMusic(it.second);
			}

			_soundEffectCache.clear();
			_musicCache.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
		auto it = _soundEffectCache.find(filePath);

		SoundEffect effect;

		if (it == _soundEffectCache.end()) {
			//add to cache and return
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
				fatalError("Failed to open audio file: " + filePath);

			_soundEffectCache[filePath] = chunk;

			effect._chunk = chunk;

			return effect;
		}

		effect._chunk = it->second;

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& filePath) {
		auto it = _musicCache.find(filePath);

		Music music;

		if (it == _musicCache.end()) {
			//add to cache and return
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
				fatalError("Failed to open audio file: " + filePath);

			_musicCache[filePath] = mixMusic;

			music._music = mixMusic;

			return music;
		}

		music._music = it->second;

		return music;
	}
}
