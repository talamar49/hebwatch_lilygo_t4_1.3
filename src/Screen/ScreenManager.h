#include "Screen/IScreen.h"

class ScreenManager
{
public:
    ScreenManager(IScreen* initialScreen_);
    
    void SetScreen(IScreen* newScreen_);
    
    void Render();
    
private:
    IScreen *m_currentScreen;
};