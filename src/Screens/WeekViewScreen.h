#pragma once 

#include <TFT_eSPI.h>
#include "Framework/Screen/IScreen.h"

class WeekViewScreen : public IScreen 
{
public:
    WeekViewScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 8);

    void Render() override;

    void UpdateHebdates(std::vector<String> hebdates_);
    void UpdateJuldates(std::vector<String> juldates_);
    void UpdateSunrise(std::vector<String> sunrises_);
    void UpdateSunset(std::vector<String> sunsets_);
    void UpdateAlot(std::vector<String> alots_);
    void UpdatePlag(std::vector<String> plags_);
    void UpdateShma(std::vector<String> shmas_);
    void UpdateHatzot(std::vector<String> hatzots_);
    void UpdateShabat(const String shabat_start_, String shabat_end_, String shabat_tam_);
    void UpdateCurrHebdate(std::vector<String> curr_hebdate_);
    void UpdateCurrJuldate(std::vector<String> curr_juldate_);
    void UpdateStandardTime(const String &standardtime_);
    void UpdateHebtime(const String &hebtime_);
    void UpdateCity(String &city_);
    void UpdateParasha(const String &parash_);

    void TFTInitContent(void);
    void TFTInitUIFrame(void);
    
private:
    TFT_eSPI m_tft;
    TFT_eFEX m_fex;
    
    TFT_eSprite m_zmanim_sprite;
    TFT_eSprite m_jultime_sprite;
    TFT_eSprite m_standardTime_sprite;
    TFT_eSprite m_juldate_sprite;

};