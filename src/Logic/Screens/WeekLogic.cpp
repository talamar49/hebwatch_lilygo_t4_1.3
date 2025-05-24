// WeekLogic.cpp
#include "WeekLogic.h"
#include <EEPROM.h>

WeekLogic::WeekLogic(TopicServer& topicServer)
    : m_topicServer(topicServer)
{
    m_currentCity = LoadCityFromEEPROM();
    m_cityIt = CITY_MAP.find(m_currentCity);
    if (m_cityIt == CITY_MAP.end()) {
        m_cityIt = CITY_MAP.begin();
    }

    m_topicServer.Subscribe<DateTime>("DateTimeTopic", [this](DateTime now) {
        m_datetimeNow = now;
    });

    m_topicServer.Subscribe("LeftButtonLongPressTopic", [this]() {
        OnLeftButtonLongPress();
    });
    m_topicServer.Subscribe("RightButtonLongPressTopic", [this]() {
        OnRightButtonLongPress();
    });
    m_topicServer.Subscribe("LeftButtonShortPressTopic", [this]() {
        OnLeftButtonShortPress();
    });
    m_topicServer.Subscribe("RightButtonShortPressTopic", [this]() {
        OnRightButtonShortPress();
    });
    m_topicServer.Subscribe("MiddleButtonShortPressTopic", [this]() {
        OnMiddleButtonShortPress();
    });
}

void WeekLogic::OnLoop() {}

void WeekLogic::OnMiddleButtonShortPress() {
    if (m_bIsInCityCalib) {
        m_bIsInCityCalib = false;
        m_currentCity = m_cityIt->first;
        SaveCityToEEPROM(m_currentCity);
        m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_cityIt->second);
    } else if (m_bIsInDateTimeCalib) {
        m_timeAndDateCalibIndex++;
        if (m_timeAndDateCalibIndex > 5) {
            m_bIsInDateTimeCalib = false;
            m_timeAndDateCalibIndex = 0;
            m_datetimeNow = m_tempDateTime;
            m_topicServer.Publish<DateTime>("AdjustDateTimeTopic", m_datetimeNow);
        }
    }
}

void WeekLogic::OnMiddleButtonLongPress() {}

void WeekLogic::OnRightButtonShortPress() {
    if (IsInCalib()) {
        if (m_bIsInCityCalib) {
            if (++m_cityIt == CITY_MAP.end()) m_cityIt = CITY_MAP.begin();
            m_currentCity = m_cityIt->first;
            m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_cityIt->second);
        } else if (m_bIsInDateTimeCalib) {
            switch (m_timeAndDateCalibIndex) {
                case 1: m_tempDateTime = m_tempDateTime + TimeSpan(0, 1, 0, 0); break;
                case 2: m_tempDateTime = m_tempDateTime + TimeSpan(0, 0, 1, 0); break;
                case 3: m_tempDateTime = m_tempDateTime + TimeSpan(1, 0, 0, 0); break;
                case 4: AdjustMonth(1); break;
                case 5: AdjustYear(1); break;
            }
        }
    }
}

void WeekLogic::OnRightButtonLongPress() {
    if (!IsInCalib()) {
        m_bIsInDateTimeCalib = true;
        m_tempDateTime = m_datetimeNow;
        m_timeAndDateCalibIndex = 1;
    }
}

void WeekLogic::OnLeftButtonShortPress() {
    if (IsInCalib()) {
        if (m_bIsInCityCalib) {
            if (m_cityIt == CITY_MAP.begin()) m_cityIt = CITY_MAP.end();
            --m_cityIt;
            m_currentCity = m_cityIt->first;
            m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_cityIt->second);
        } else if (m_bIsInDateTimeCalib) {
            switch (m_timeAndDateCalibIndex) {
                case 1: m_tempDateTime = m_tempDateTime + TimeSpan(0, -1, 0, 0); break;
                case 2: m_tempDateTime = m_tempDateTime + TimeSpan(0, 0, -1, 0); break;
                case 3: m_tempDateTime = m_tempDateTime + TimeSpan(-1, 0, 0, 0); break;
                case 4: AdjustMonth(-1); break;
                case 5: AdjustYear(-1); break;
            }
        }
    }
}

void WeekLogic::OnLeftButtonLongPress() {
    if (!IsInCalib()) {
        m_bIsInCityCalib = true;
        m_cityIt = CITY_MAP.find(m_currentCity);
        if (m_cityIt == CITY_MAP.end()) m_cityIt = CITY_MAP.begin();
        m_currentCity = m_cityIt->first;
        m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_cityIt->second);
    }
}

bool WeekLogic::IsInCalib() const {
    return m_bIsInDateTimeCalib || m_bIsInCityCalib;
}

bool WeekLogic::IsInCityCalib() const { return m_bIsInCityCalib; }
bool WeekLogic::IsInDateTimeCalib() const { return m_bIsInDateTimeCalib; }
const DateTime& WeekLogic::GetCurrentDateTime() const { return m_datetimeNow; }
const String& WeekLogic::GetCurrentCity() const { return m_currentCity; }
CityCoord WeekLogic::GetCurrentCityCoord() const { return m_cityIt->second; }
const DateTime& WeekLogic::GetTempDateTime() const { return m_tempDateTime; }
unsigned int WeekLogic::GetCalibIndex() const { return m_timeAndDateCalibIndex; }

String WeekLogic::LoadCityFromEEPROM() {
    int len = EEPROM.read(EEPROM_CITY_ADDR);
    if (len <= 0 || len > MAX_CITY_LENGTH) return CITY_MAP.begin()->first;
    String city;
    for (int i = 0; i < len; i++) {
        city += static_cast<char>(EEPROM.read(EEPROM_CITY_ADDR + 1 + i));
    }
    return city;
}

void WeekLogic::SaveCityToEEPROM(const String& city) {
    for (int i = 0; i < MAX_CITY_LENGTH + 1; i++) EEPROM.write(EEPROM_CITY_ADDR + i, 0);
    EEPROM.write(EEPROM_CITY_ADDR, city.length());
    for (int i = 0; i < city.length(); i++) EEPROM.write(EEPROM_CITY_ADDR + 1 + i, city[i]);
    EEPROM.commit();
}

void WeekLogic::AdjustMonth(int delta) {
    uint16_t year = m_tempDateTime.year();
    uint8_t month = m_tempDateTime.month() + delta;
    uint8_t day = m_tempDateTime.day();
    if (delta > 0) while (month > 12) { month -= 12; year++; }
    else while (month < 1) { month += 12; year--; }
    uint8_t maxDay = DaysInGivenMonth(year, month);
    if (day > maxDay) day = maxDay;
    m_tempDateTime = DateTime(year, month, day, m_tempDateTime.hour(), m_tempDateTime.minute(), m_tempDateTime.second());
}

void WeekLogic::AdjustYear(int delta) {
    uint16_t year = m_tempDateTime.year() + delta;
    uint8_t month = m_tempDateTime.month();
    uint8_t day = m_tempDateTime.day();
    uint8_t maxDay = DaysInGivenMonth(year, month);
    if (day > maxDay) day = maxDay;
    m_tempDateTime = DateTime(year, month, day, m_tempDateTime.hour(), m_tempDateTime.minute(), m_tempDateTime.second());
}

uint8_t WeekLogic::DaysInGivenMonth(uint16_t year, uint8_t month) {
    static const uint8_t days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (month == 2 && IsLeapYear(year)) ? 29 : days[month - 1];
}

bool WeekLogic::IsLeapYear(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
