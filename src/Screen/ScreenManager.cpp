#include "Screen/ScreenManager.h"

ScreenManager::ScreenManager(IScreen* initialScreen_)
                             : m_currentScreen(initialScreen_)
{
    m_currentScreen->Render();
}

void ScreenManager::SetScreen(IScreen* newScreen_) 
{
    m_currentScreen = newScreen_;
}

void ScreenManager::Render() 
{
    m_currentScreen->Render();
}