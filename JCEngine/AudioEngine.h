#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace JCEngine {
	class SoundEffect {
	public:
		friend class AudioEngine;

		//@param loops: if loops = -1, loop forever. otherwise, play loops + 1 times
		void play(int loops = 0);

	private:
		Mix_Chunk* _chunk = nullptr;
	};

	class Music {
	public:
		friend class AudioEngine;

		//@param loops: if loops = -1, loop forever. otherwise, play loops times
		void play(int loops = 1);

		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* _music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		bool _isInitialized = false;

		std::map<std::string, Mix_Chunk*> _soundEffectCache;
		std::map<std::string, Mix_Music*> _musicCache;
	};
}


