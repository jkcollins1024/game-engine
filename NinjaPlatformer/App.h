#pragma once

#include <JCEngine/IMainGame.h>
#include "GameplayScreen.h"

class App : public JCEngine::IMainGame
{
	public:
		App();
		~App();

		virtual void onInit() override;
		virtual void addScreens() override;
		virtual void onExit() override;
	private:
		GameplayScreen* m_gameplayScreen = nullptr;
};

