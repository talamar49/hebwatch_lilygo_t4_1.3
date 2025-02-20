#include "Framework/Screen/IScreen.h"

class ScreenManager
{
public:
    ScreenManager(IScreen* initialScreen_);
    
    void SetScreen(IScreen* newScreen_);
    
    void Render();

    IScreen* GetCurrentScreen();
    
private:
    IScreen *m_currentScreen;
};