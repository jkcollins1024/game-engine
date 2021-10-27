#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace JCEngine {

	class IMainGame;

	enum class ScreenState{NONE, EXIT, RUNNING, CHANGE_NEXT, CHANGE_PREVIOUS};
	class IGameScreen
	{
		friend class ScreenList;
	public:
		IGameScreen() {}
		virtual ~IGameScreen() {}

		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		//call at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;

		//called when we enter and exit a screen
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//called in main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		int getScreenIndex() const { return m_screenIndex; }
		ScreenState getState() const { return m_currentState; }

		void setRunning() { m_currentState = ScreenState::RUNNING; }
		void setParentGame(IMainGame* game) { m_game = game; }

	protected:
		int m_screenIndex = -1;
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_game = nullptr;
	};
}


