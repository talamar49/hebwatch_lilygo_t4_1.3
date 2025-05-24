#pragma once
#include <RTClib.h>
#include <map>
#include "Utils/DataNeeded.h"
#include "Framework/Data/TopicServer.h"


class WeekLogic {
public:
    WeekLogic(TopicServer& topicServer);

    void OnLoop();
    void OnMiddleButtonShortPress();
    void OnMiddleButtonLongPress();
    void OnRightButtonShortPress();
    void OnRightButtonLongPress();
    void OnLeftButtonShortPress();
    void OnLeftButtonLongPress();

    const DateTime& GetCurrentDateTime() const;
    const String& GetCurrentCity() const;
    CityCoord GetCurrentCityCoord() const;
    bool IsInCalib() const;
    bool IsInCityCalib() const;
    bool IsInDateTimeCalib() const;
    const DateTime& GetTempDateTime() const;
    unsigned int GetCalibIndex() const;

private:
    TopicServer& m_topicServer;
    DateTime m_datetimeNow;
    DateTime m_tempDateTime;
    bool m_bIsInDateTimeCalib = false;
    bool m_bIsInCityCalib = false;
    unsigned int m_timeAndDateCalibIndex = 0;

    String m_currentCity;
    std::map<String, CityCoord>::const_iterator m_cityIt;

    static constexpr int EEPROM_CITY_ADDR = 0;
    static constexpr int MAX_CITY_LENGTH = 40;

    const std::map<String, CityCoord> CITY_MAP = {
        { "ירושלים", {31.7683, 35.2137} },
        { "חיפה",    {32.7940, 34.9896} },
        { "תל אביב", {32.0853, 34.7818} },
        { "באר שבע", {31.2518, 34.7913} }
    };

    String LoadCityFromEEPROM();
    void SaveCityToEEPROM(const String& city);
    void AdjustMonth(int delta);
    void AdjustYear(int delta);
    uint8_t DaysInGivenMonth(uint16_t year, uint8_t month);
    bool IsLeapYear(uint16_t year);
};
