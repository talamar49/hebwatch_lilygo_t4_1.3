#include <UIRenderer.h>

#include "Rubik-Light10.h"
#include "Rubik-Light12.h"
#include "Rubik-Light14.h"
#include "Rubik-Light20.h"
#include "Rubik-Light26.h"
#include "Rubik-Light28.h"


void UIRenderer::UpdateCurrHebdate(const String currHebdate_[])
{
  String hebdate = currHebdate_[0]+" "+currHebdate_[1]+" "+currHebdate_[2];
  WriteString(hebdate, TFT_WIDTH - 1, 25, Rubik_Light14 , TextDirection::RTL);
}

void UIRenderer::UpdateCurrJuldate(const String currJuldate_[])
{
  String juldate = currJuldate_[0]+"/"+currJuldate_[1]+"/"+currJuldate_[2];
  WriteString(juldate, 62, 25, Rubik_Light14 , TextDirection::LTR);
}

void UIRenderer::UpdateHebtime(const String &hebTime_)
{
  WriteString(hebTime_, 147, 5, Rubik_Light26, TextDirection::LTR);
}

void UIRenderer::UpdateStandardTime(const String &standardtime_)
{
  WriteString(standardtime_, 70, 5, Rubik_Light26, TextDirection::LTR);
}

void UIRenderer::UpdateCity(const String &city_)
{
  WriteString(city_, 58, 10, Rubik_Light20, TextDirection::RTL);
}

void UIRenderer::UpdateHebdates(const String hebDates_[])
{
  WriteToCols(hebDates_, 216, 5, Rubik_Light14, TextDirection::RTL); 
}

void UIRenderer::UpdateJuldates(const String julDates_[])
{
  WriteToCols(julDates_, 201, 20, Rubik_Light14, TextDirection::LTR);
}

void UIRenderer::UpdateShabat(const String shabatStart_, String shabatEnd_, String shabatTam_)
{
  WriteString(shabatStart_, 110, 243, Rubik_Light12, TextDirection::LTR);
  WriteString(shabatEnd_, 110, 283, Rubik_Light12, TextDirection::LTR);
  WriteString(shabatTam_, 110, 295, Rubik_Light12, TextDirection::LTR);
  WriteString("כש", 160, 243, Rubik_Light12, TextDirection::RTL);
  WriteString("צש", 160, 283, Rubik_Light12, TextDirection::RTL);
  WriteString("רת", 160, 295, Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdateAlot(const String alots_[])
{
  int yaxis = 3;
  static String alot_notation[] = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  WriteToCols(alots_, 54, yaxis, Rubik_Light12, TextDirection::LTR);
  WriteToCols(alot_notation, 105, yaxis,Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdateSunrise(const String sunrises[])
{
  int yaxis = 15;
  static String sunrise_notation[] = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};

  WriteToCols(sunrises, 54, yaxis, Rubik_Light12, TextDirection::LTR); 
  WriteToCols(sunrise_notation, 105, yaxis,Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdateShma(const String shmas[])
{
  int yaxis = 27;
  static String shma_notation[] = {"קש", "קש", "קש", "קש", "קש","קש", "קש"};

  WriteToCols(shmas, 54, yaxis, Rubik_Light12, TextDirection::LTR);
  WriteToCols(shma_notation, 105, yaxis,Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdateHatzot(const String hatzots[])
{
  int yaxis = 3;
  static String hatzot_notation[] = {"ח", "ח", "ח", "ח", "ח","ח", "ח"};

  WriteToCols(hatzots, 3, yaxis, Rubik_Light12, TextDirection::LTR);
  WriteToCols(hatzot_notation, 48, yaxis,Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdatePlag(const String plags[])
{
  int yaxis = 15;
  static String plag_notation[] = {"פ", "פ", "פ", "פ", "פ","פ", "פ"};

  WriteToCols(plags, 3, yaxis, Rubik_Light12, TextDirection::LTR);
  WriteToCols(plag_notation, 48, yaxis,Rubik_Light12, TextDirection::RTL);
}

void UIRenderer::UpdateSunset(const String sunsets[])
{
  int yaxis = 27;
  static String sunset_notation[] = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};

  WriteToCols(sunsets, 3, yaxis, Rubik_Light12, TextDirection::LTR); 
  WriteToCols(sunset_notation, 48, yaxis,Rubik_Light12, TextDirection::RTL);
}





void UIRenderer::WriteToCols(const String str_arr[], int32_t x, int32_t y, const uint8_t *font, TextDirection textDir_)
{
  m_tft->loadFont(font);

  int i = 1;
  int lines = 8;

  for(; i < lines; ++i)
  {
    int portion = (TFT_HEIGHT / lines) * (i);

    if(textDir_ == TextDirection::RTL)
    {
        m_fex->setCursorRTL(x ,portion + y);
        m_fex->drawStringRTL(str_arr[i-1]);
    }else{
        m_tft->drawString(str_arr[i-1],x, portion + y);
    }
  }

  m_tft->unloadFont();
}

void UIRenderer::WriteString(const String &str, int32_t x, int32_t y, const uint8_t *font, TextDirection textDir_)
{
    m_tft->loadFont(font);

    if(textDir_ == TextDirection::RTL)
    {
        m_fex->setCursorRTL(x, y);
        m_fex->drawStringRTL(str);
    }else{
        m_tft->drawString(str, x, y);
    }

    m_tft->unloadFont();
}