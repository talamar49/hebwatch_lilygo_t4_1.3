#pragma once 

#include <TFT_eSPI.h>
#include <RTClib.h>
#include "Framework/Screen/IScreen.h"
#include "Framework/Data/TopicServer.h"

class SettingsScreen : public IScreen 
{
public:
    SettingsScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 8);

    void Render() override;
    void Loop() override;

    void TFTInitContent(void);
    void TFTInitUIFrame(void);
    
private:
    TFT_eSPI m_tft;
    TFT_eFEX m_fex;
    TFT_eSprite m_standardTime_sprite;
    TFT_eSprite m_juldate_sprite;

    DateTime m_datetimeNow;
    unsigned int m_selectionIndex;

    void UpdateStandardTime(const String &standardtime_);
    void UpdateCurrJuldate(std::vector<String> curr_juldate_);
    void ChangeSelection();
};