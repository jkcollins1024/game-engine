#include "IMainGame.h"
#include "Timing.h"
#include "JCEngine.h"

namespace JCEngine {
	IMainGame::IMainGame()
	{
		m_screenList = new ScreenList(this);
	}

	IMainGame::~IMainGame()
	{
	}

	void IMainGame::run()
	{
		if (!init())
			return;

		FPSLimiter limiter;
		limiter.setMaxFPS(60); //max FPS of 60 for now
		
		m_isRunning = true;
		while (m_isRunning) {
			limiter.start();

			inputManager.update();

			update();
			if (!m_isRunning)
				break;
			draw();

			m_FPS = limiter.stop();
			m_window.swapBuffer();
		}		
	}

	void IMainGame::exit()
	{
		m_currentScreen->onExit();

		if (m_screenList) {
			m_screenList->~ScreenList();
			delete m_screenList;
		}
		m_isRunning = false;
	}

	bool IMainGame::init()
	{
		JCEngine::init();

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		if (!initSystems()) return false;

		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}

	bool IMainGame::initSystems() {
		m_window.create("Default Window", 1024, 768, 0);

		return true;
	}

	void IMainGame::update() {
		if (m_currentScreen) {
			switch (m_currentScreen->getState()) {
				case ScreenState::RUNNING:
					m_currentScreen->update();
					break;
				case ScreenState::CHANGE_NEXT:
					m_currentScreen->onExit();
					m_currentScreen = m_screenList->moveNext();
					if (m_currentScreen) {
						m_currentScreen->setRunning();
						m_currentScreen->onEntry();
					}
					break;
				case ScreenState::CHANGE_PREVIOUS:
					m_currentScreen->onExit();
					m_currentScreen = m_screenList->movePrevious();
					if (m_currentScreen) {
						m_currentScreen->setRunning();
						m_currentScreen->onEntry();
					}
					break;
				case ScreenState::EXIT:
					exit();
					break;
				case ScreenState::NONE:
				default:
					break;

			}
		}
		else {
			exit();
		}
	}

	void IMainGame::draw() {
		glViewport(0, 0, m_window.GetScreenWidth(), m_window.GetScreenHeight());

		if (m_currentScreen) {
			if (m_currentScreen->getState() == ScreenState::RUNNING)
				m_currentScreen->draw();
		}
		else {
			m_currentScreen->draw();
		}
	}

	void IMainGame::onSDLEvent(SDL_Event& event) {
		switch (event.type) {
		case SDL_QUIT:
			exit();
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.keyPress(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.keyRelease(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.keyPress(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.keyRelease(event.button.button);
			break;
		}
	}

}


