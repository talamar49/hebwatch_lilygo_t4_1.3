#pragma once

#include <Arduino.h> // arduino functions
#include <TFT_eSPI.h> // spi function
#include <TFT_eFEX.h> // Include the extension graphics functions library
#include <iostream>
#include <memory>



class UIRenderer
{
public:
    enum class TextDirection
    {
        LTR = 0,
        RTL = 1
    };

    struct HebWeekData
    {
        String city;            // "ירושלים"
        String hebtime;         // "01:23"     
        String standardtime;    // "20:46"
        String currHebdate[3];  // {"כה","ניסן","תשגד"}
        String currJuldate[3];  // {"12","04","2024"}
        String hebdates[7];     // {"כה", "כו", "כז", "כח", "כט","ל", "א"}
        String juldates[7];     // {"12", "13", "14", "15", "16","17","18"}
        String sunrise[7];      // {"06:42", "06:43", "06:45", "06:48", "06:48","06:50","06:51"}
        String alot[7];         // ""
        String sunset[7];       // ""
        String shma[7];         // ""
        String hatzot[7];       // ""
        String plag[7];         // ""
        String shabatStart;     // "18:52"
        String shabatEnd;       // ""
        String shabatTam;       // ""
        String parasha;         // "אחרי מות - קדושים"
    }; 

    UIRenderer(HebWeekData* dataStruct_, TFT_eFEX* fex_, TFT_eSPI* tft_);

    void TFTInitContent(void);

    void UpdateHebdates(const String hebdates_[]);
    void UpdateJuldates(const String juldates_[]);
    void UpdateSunrise(const String sunrises_[]);
    void UpdateSunset(const String sunsets_[]);
    void UpdateAlot(const String alots_[]);
    void UpdatePlag(const String plags_[]);
    void UpdateShma(const String shmas_[]);
    void UpdateHatzot(const String hatzots_[]);
    void UpdateShabat(const String shabatStart_, String shabatEnd_, String shabatTam_);
    void UpdateCurrHebdate(const String currHebdate_[]);
    void UpdateCurrJuldate(const String currJuldate_[]);
    void UpdateStandardTime(const String &standardTime_);
    void UpdateHebtime(const String &hebTime_);
    void UpdateCity(const String &city_);


private:
    HebWeekData* m_weekData;
    TFT_eSPI* m_tft;
    TFT_eFEX* m_fex; 

    void WriteString(const String &string_, int32_t x_, int32_t y_, const uint8_t *font_, TextDirection textDir_);
    void WriteToCols(const String str_arr_[], int32_t x_, int32_t y_, const uint8_t *font_, TextDirection textDir_);
    void DrawColVertLine(int x_);
};
