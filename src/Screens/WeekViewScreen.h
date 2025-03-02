#pragma once 

#include <TFT_eSPI.h>
#include <RTClib.h>
#include <iomanip>
#include <sstream>
#include <map>

#include "Utils/DataNeeded.h"
#include "Framework/Screen/IScreen.h"

class WeekViewScreen : public IScreen 
{
public:

    WeekViewScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 8);

    void Render() override;
    void Loop() override;
    
private:
    static constexpr int EEPROM_CITY_ADDR = 0;
    static constexpr int MAX_CITY_LENGTH = 40;

    TFT_eSPI m_tft;
    TFT_eFEX m_fex;
    
    TFT_eSprite m_zmanim_sprite;
    TFT_eSprite m_standardTime_sprite;
    TFT_eSprite m_juldate_sprite;
    TFT_eSprite m_julTime_sprite;
    TFT_eSprite m_julDayNMonth_sprite;
    TFT_eSprite m_julYear_sprite;
    DateTime m_datetimeNow;
    DateTime m_tempDateTime; // Store temporary DateTime during calibration

    bool m_bIsJulTimeAndDateCalib = false;
    bool m_bIsCityCalib = false;
    unsigned int m_timeAndDateCalibIndex = 0;
    int m_hourChangeCalib = 0;
    int m_minChangeCalib = 0;

    uint32_t m_calibBgColor = TFT_BROWN;

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
    void UpdateStandardTime(std::vector<String> standardtime_, uint32_t hourBg = 0, uint32_t minuteBg = 0);
    void UpdateHebtime(const String &hebtime_);
    void UpdateCity(uint32_t cityBg = 0);
    void UpdateParasha(const String &parash_);

    bool IsInCalib();
    void CalibLogic();
    void LoopLogic();

    void MiddleButtonShortPressHandler();
    void MiddleButtonLongPressHandler();
    void RightButtonShortPressHandler();
    void RightButtonLongPressHandler();
    void LeftButtonShortPressHandler();
    void LeftButtonLongPressHandler();

    void TFTInitContent(void);
    void TFTInitUIFrame(void);

    void AdjustMonth(int delta);
    void AdjustYear(int delta);
    // Include DaysInGivenMonth and IsLeapYear from earlier if not already present
    uint8_t DaysInGivenMonth(uint16_t year, uint8_t month);
    bool IsLeapYear(uint16_t year);

    String LoadCityFromEEPROM();
    void SaveCityToEEPROM(const String& city);

    const std::map<String, CityCoord> CITY_MAP = {
        { "ירושלים", {31.7683, 35.2137} },
        { "חיפה",    {32.7940, 34.9896} },
        { "תל אביב", {32.0853, 34.7818} },
        { "באר שבע", {31.2518, 34.7913} }
    };

    std::map<String, CityCoord>::const_iterator m_cityIt; // Iterator for city map
    String m_currentCity = "ירושלים"; 

};