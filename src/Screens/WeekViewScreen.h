// WeekViewScreen.h (Updated)
#pragma once

#include <TFT_eSPI.h>
#include <RTClib.h>
#include <map>
#include <vector>

#include "Utils/DataNeeded.h"
#include "Framework/Screen/IScreen.h"
#include "Logic/Screens/WeekLogic.h"
#include "Logic/ZmanimLogic/ZmanimLogic.h"


class WeekViewScreen : public IScreen 
{
public:
    WeekViewScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 8);

    void Render() override;
    void Loop() override;

private:
    TFT_eSPI m_tft;
    TFT_eFEX m_fex;

    TFT_eSprite m_zmanim_sprite;
    TFT_eSprite m_standardTime_sprite;
    TFT_eSprite m_juldate_sprite;
    TFT_eSprite m_julTime_sprite;
    TFT_eSprite m_julDayNMonth_sprite;
    TFT_eSprite m_julYear_sprite;
    TFT_eSprite m_delim_sprite;
    uint32_t m_calibBgColor = TFT_BROWN;

    WeekLogic m_weekLogic;
    ZmanimLogic m_zmanimLogic;

    uint32_t m_hourBg = 0, m_minuteBg = 0, m_dayBg = 0, m_monthBg = 0, m_yearBg = 0;
    String m_hebrewDate;

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
    void UpdateCurrJuldate(std::vector<String> curr_juldate_, uint32_t dayBg = 0, uint32_t monthBg = 0, uint32_t yearBg = 0);
    void UpdateStandardTime(std::vector<String> standardtime_, uint32_t hourBg = 0, uint32_t minuteBg = 0, uint8_t currSeconds = 0);
    void UpdateHebtime(const String &hebtime_);
    void UpdateCity(uint32_t cityBg = 0);
    void UpdateParasha(const String &parash_);

    void UpdateCalibrationLogic();

    void TFTInitContent();
    void TFTInitUIFrame();

    std::vector<String> SplitStringBySpace(const String& input);
    std::vector<DateTime> GetWeekDatetimeFromCurrentDay(DateTime current);
    std::vector<String> GetHebrewDatesForWeek(const std::vector<ZmanimData>& zmanimWeek);
    std::vector<String> GetJulDatesForWeek(const std::vector<DateTime>& weekDatetime);

};