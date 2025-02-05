#include "WeekViewScreen.h"

void WeekViewScreen::Render()
{
    
}

WeekViewScreen::WeekViewScreen(TFT_eSPI tft_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                                : IScreen(bgColor_, textColor_, lineColor_, numOfLines_),
                                  m_tft(tft_),
                                  m_fex(&m_tft),
                                  m_juldate_sprite(&m_tft),
                                  m_jultime_sprite(&m_tft),
                                  m_standardTime_sprite(&m_tft),
                                  m_zmanim_sprite(&m_tft)
{}

void WeekViewScreen::UpdateCurrHebdate(std::vector<const String> curr_hebdate_)
{
  String hebdate = curr_hebdate_[0]+" "+curr_hebdate_[1]+" "+curr_hebdate_[2];
  
  WriteString(StringObj(hebdate, TFT_WIDTH - 1, 25, 90, 15, Rubik_Light14), m_fex);
}

void WeekViewScreen::UpdateCurrJuldate(std::vector<const String> curr_juldate_)
{
  String juldate = curr_juldate_[0]+"/"+curr_juldate_[1]+"/"+curr_juldate_[2];
  WriteSpriteString(StringObj(juldate, 62, 25, 77, 13, Rubik_Light14), m_jultime_sprite);
}

void WeekViewScreen::UpdateHebtime(const String &hebtime_)
{
  WriteSpriteString(StringObj(hebtime_, 147, 5, 68, 18, Rubik_Light26), m_standardTime_sprite);
}

void WeekViewScreen::UpdateStandardTime(const String &standardtime_)
{
  WriteSpriteString(StringObj(standardtime_, 70, 5, 68, 18, Rubik_Light26), m_standardTime_sprite);
}

void WeekViewScreen::UpdateCity(const String &city_)
{
  WriteString(StringObj(city_, 58, 10, 55, 20, Rubik_Light20), m_fex);
}

void WeekViewScreen::UpdateHebdates(std::vector<const String> hebdates_)
{
  WriteToCols(StringArrObj(hebdates_, 216, 5, 18, 13, Rubik_Light14), m_fex); 
}

void WeekViewScreen::UpdateJuldates(std::vector<const String> juldates_)
{
  WriteSpriteToCols(StringArrObj(juldates_, 201, 20, 19, 13, Rubik_Light14), m_juldate_sprite);
}

void WeekViewScreen::UpdateShabat(const String shabat_start_, String shabat_end_, String shabat_tam_)
{
  WriteSpriteString(StringObj(shabat_start_, 110, 243, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteSpriteString(StringObj(shabat_end_, 110, 283, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteSpriteString(StringObj(shabat_tam_, 110, 295, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteString(StringObj("כש", 160, 243, 5, 5, Rubik_Light12), m_fex);
  WriteString(StringObj("צש", 160, 283, 5, 5, Rubik_Light12), m_fex);
  WriteString(StringObj("רת", 160, 295, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateAlot(std::vector<const String> alots_)
{
  int yaxis = 3;
  static std::vector<const String> alot_notation = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  WriteSpriteToCols(StringArrObj(alots_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(alot_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateSunrise(std::vector<const String> sunrises_)
{
  int yaxis = 15;
  static std::vector<const String> sunrise_notation = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};

  WriteSpriteToCols(StringArrObj(sunrises_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite); 
  WriteToCols(StringArrObj(sunrise_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateShma(std::vector<const String> shmas_)
{
  int yaxis = 27;
  static std::vector<const String> shma_notation = {"קש", "קש", "קש", "קש", "קש","קש", "קש"};

  WriteSpriteToCols(StringArrObj(shmas_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(shma_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateHatzot(std::vector<const String> hatzots_)
{
  int yaxis = 3;
  static std::vector<const String> hatzot_notation = {"ח", "ח", "ח", "ח", "ח","ח", "ח"};

  WriteSpriteToCols(StringArrObj(hatzots_, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(hatzot_notation, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdatePlag(std::vector<const String> plags_)
{
  int yaxis = 15;
  static std::vector<const String> plag_notation = {"פ", "פ", "פ", "פ", "פ","פ", "פ"};

  WriteSpriteToCols(StringArrObj(plags_, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(plag_notation, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateSunset(std::vector<const String> sunsets)
{
  int yaxis = 27;
  static std::vector<const String> sunset_notation_ = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};

  WriteSpriteToCols(StringArrObj(sunsets, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite); 
  WriteToCols(StringArrObj(sunset_notation_, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}