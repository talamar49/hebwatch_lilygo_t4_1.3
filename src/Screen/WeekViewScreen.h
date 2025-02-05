#include <TFT_eSPI.h>
#include "IScreen.h"

class WeekViewScreen : public IScreen 
{
public:
    WeekViewScreen(TFT_eSPI tft_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 8);

    void Render() override;

    void UpdateHebdates(std::vector<const String> hebdates_);
    void UpdateJuldates(std::vector<const String> juldates_);
    void UpdateSunrise(std::vector<const String> sunrises_);
    void UpdateSunset(std::vector<const String> sunsets_);
    void UpdateAlot(std::vector<const String> alots_);
    void UpdatePlag(std::vector<const String> plags_);
    void UpdateShma(std::vector<const String> shmas_);
    void UpdateHatzot(std::vector<const String> hatzots_);
    void UpdateShabat(const String shabat_start_, String shabat_end_, String shabat_tam_);
    void UpdateCurrHebdate(std::vector<const String> curr_hebdate_);
    void UpdateCurrJuldate(std::vector<const String> curr_juldate_);
    void UpdateStandardTime(const String &standardtime_);
    void UpdateHebtime(const String &hebtime_);
    void UpdateCity(const String &city_);
    
private:
    TFT_eSPI m_tft;
    TFT_eFEX m_fex;
    
    TFT_eSprite m_zmanim_sprite;
    TFT_eSprite m_jultime_sprite;
    TFT_eSprite m_standardTime_sprite;
    TFT_eSprite m_juldate_sprite;

};