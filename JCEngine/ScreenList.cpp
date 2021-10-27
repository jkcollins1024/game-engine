#include "ScreenList.h"

namespace JCEngine {
    ScreenList::ScreenList(IMainGame* mainGame)
    {
        m_game = mainGame;
    }

    ScreenList::~ScreenList()
    {
        for (size_t i = 0; i < m_screens.size(); i++) {
            m_screens[i]->destroy();
            ///delete m_screens[i]; managing elsewhere
        }

        m_screens.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::moveNext()
    {
        IGameScreen* currentScreen = getCurrent();

        if (currentScreen != nullptr && currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
            m_currentScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen* ScreenList::movePrevious()
    {
        IGameScreen* currentScreen = getCurrent();

        if (currentScreen != nullptr && currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
            m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen* ScreenList::getCurrent() {
        if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN)
            return nullptr;

        return m_screens[m_currentScreenIndex];
    }

    void ScreenList::setScreen(int newScreen)
    {
        m_currentScreenIndex = newScreen;
    }

    void ScreenList::addScreen(IGameScreen* newScreen)
    {
        newScreen->m_screenIndex = m_screens.size();
        m_screens.push_back(newScreen);
        newScreen->build();
        newScreen->setParentGame(m_game);
    }
}