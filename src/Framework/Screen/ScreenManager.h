#include "Framework/Screen/IScreen.h"

class ScreenManager
{
public:
    ScreenManager(IScreen* initialScreen_);
    
    void SetScreen(IScreen* newScreen_);
    
    void Render();

    void Loop();

    IScreen* GetCurrentScreen();
    
private:
    IScreen *m_currentScreen;
};