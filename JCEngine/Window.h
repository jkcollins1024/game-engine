#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace JCEngine {
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string name, int screenWidth, int screenHeight, unsigned int windowFlags);

		void setColor(float r, float g, float b, float a);

		void swapBuffer();

		int GetScreenWidth() { return _screenWidth; };
		int GetScreenHeight() { return _screenHeight; };

	private:
		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;
	};

}

