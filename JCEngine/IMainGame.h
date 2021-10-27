#pragma once
#include "ScreenList.h"
#include "Window.h"
#include "InputManager.h"

namespace JCEngine {
	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exit();

		virtual void onInit() = 0;
		virtual void addScreens() = 0;
		virtual void onExit() = 0;

		const float getFPS() const { return m_FPS; }

		void onSDLEvent(SDL_Event& event);

		InputManager inputManager;

	protected:
		ScreenList* m_screenList;
		IGameScreen* m_currentScreen = nullptr;
		bool m_isRunning = false;
		float m_FPS = 0;
		Window m_window;

		bool init();
		bool initSystems();
		virtual void update();
		virtual void draw();
	};
}


