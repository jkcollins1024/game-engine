#include "App.h"

App::App() {

}

App::~App() {

}

void App::onInit() {

}

void App::addScreens() {
	m_gameplayScreen = new GameplayScreen(&m_window);

	m_screenList->addScreen(m_gameplayScreen);
	m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
}

void App::onExit() {

}