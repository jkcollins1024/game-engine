#pragma once
#include <vector>
#include "IGameScreen.h"

namespace JCEngine {
	class ScreenList
	{
	public:
		ScreenList(IMainGame* mainGame);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();
		IGameScreen* getCurrent();

		void setScreen(int newScreen);
		void addScreen(IGameScreen* newScreen);
	protected:
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
		IMainGame* m_game = nullptr;
	};
}

