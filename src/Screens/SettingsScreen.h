#pragma once 

#include <TFT_eSPI.h>
#include "Framework/Screen/IScreen.h"
#include "Framework/Data/TopicServer.h"

class SettingsScreen : public IScreen 
{
public:
    SettingsScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_ = TFT_BLUE, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 0);

    void Render() override;

    void TFTInitContent(void);
    void TFTInitUIFrame(void);
    
private:
    TFT_eSPI m_tft;
    TFT_eFEX m_fex;
};