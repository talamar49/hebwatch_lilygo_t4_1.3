#include "Framework/Screen/ScreenManager.h"

ScreenManager::ScreenManager(IScreen* initialScreen_)
                             : m_currentScreen(initialScreen_)
{
    m_currentScreen->Render();
}

void ScreenManager::SetScreen(IScreen* newScreen_) 
{
    m_currentScreen = newScreen_;
    m_currentScreen->Render();
}

void ScreenManager::Render() 
{
    m_currentScreen->Render();
}

void ScreenManager::Loop() 
{
    m_currentScreen->Loop();
}

IScreen* ScreenManager::GetCurrentScreen()
{
    return m_currentScreen;
}