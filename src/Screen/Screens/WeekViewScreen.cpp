#include "Screen/Screens/WeekViewScreen.h"


WeekViewScreen::WeekViewScreen(TFT_eSPI tft_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                                : IScreen(bgColor_, textColor_, lineColor_, numOfLines_),
                                  m_tft(tft_),
                                  m_fex(&m_tft),
                                  m_juldate_sprite(&m_tft),
                                  m_jultime_sprite(&m_tft),
                                  m_standardTime_sprite(&m_tft),
                                  m_zmanim_sprite(&m_tft)
{}

void WeekViewScreen::Render()
{
  m_tft.setTextColor(m_textColor, m_bgColor);
  m_tft.fillScreen(m_bgColor);
    
  TFTInitUIFrame();
  TFTInitContent();
}

void WeekViewScreen::UpdateCurrHebdate(std::vector<String> curr_hebdate_)
{
  String hebdate = curr_hebdate_[0]+" "+curr_hebdate_[1]+" "+curr_hebdate_[2];
  
  WriteString(StringObj(hebdate, TFT_WIDTH - 1, 25, 90, 15, Rubik_Light14), m_fex);
}

void WeekViewScreen::UpdateCurrJuldate(std::vector<String> curr_juldate_)
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

void WeekViewScreen::UpdateCity(String &city_)
{
  WriteString(StringObj(city_, 58, 10, 55, 20, Rubik_Light20), m_fex);
}

void WeekViewScreen::UpdateHebdates(std::vector<String> hebdates_)
{
  WriteToCols(StringArrObj(hebdates_, 216, 5, 18, 13, Rubik_Light14), m_fex); 
}

void WeekViewScreen::UpdateJuldates(std::vector<String> juldates_)
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

void WeekViewScreen::UpdateAlot(std::vector<String> alots_)
{
  int yaxis = 3;
  static std::vector<String> alot_notation = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  WriteSpriteToCols(StringArrObj(alots_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(alot_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateSunrise(std::vector<String> sunrises_)
{
  int yaxis = 15;
  static std::vector<String> sunrise_notation = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};

  WriteSpriteToCols(StringArrObj(sunrises_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite); 
  WriteToCols(StringArrObj(sunrise_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateShma(std::vector<String> shmas_)
{
  int yaxis = 27;
  static std::vector<String> shma_notation = {"קש", "קש", "קש", "קש", "קש","קש", "קש"};

  WriteSpriteToCols(StringArrObj(shmas_, 54, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(shma_notation, 105, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateHatzot(std::vector<String> hatzots_)
{
  int yaxis = 3;
  static std::vector<String> hatzot_notation = {"ח", "ח", "ח", "ח", "ח","ח", "ח"};

  WriteSpriteToCols(StringArrObj(hatzots_, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(hatzot_notation, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdatePlag(std::vector<String> plags_)
{
  int yaxis = 15;
  static std::vector<String> plag_notation = {"פ", "פ", "פ", "פ", "פ","פ", "פ"};

  WriteSpriteToCols(StringArrObj(plags_, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite);
  WriteToCols(StringArrObj(plag_notation, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateSunset(std::vector<String> sunsets)
{
  int yaxis = 27;
  static std::vector<String> sunset_notation_ = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};

  WriteSpriteToCols(StringArrObj(sunsets, 3, yaxis, 32, 10, Rubik_Light12), m_zmanim_sprite); 
  WriteToCols(StringArrObj(sunset_notation_, 48, yaxis, 5, 5, Rubik_Light12), m_fex);
}

void WeekViewScreen::UpdateParasha(const String &parash_)
{
  WriteString(StringObj(parash_, 195, 256, 86, 12, Rubik_Light12), m_fex);
}

void WeekViewScreen::TFTInitContent(void)
{
    // init struct
  String city = "ירושלים";
  String hebtime = "01:23";
  String standardtime = "20:46";
  std::vector<String> curr_hebdate = {"כה","ניסן","תשגד"};
  std::vector<String> curr_juldate = {"12","04","2024"};
  std::vector<String> hebdates = {"כה", "כו", "כז", "כח", "כט","ל", "א"};
  std::vector<String> juldates = {"00", "13", "14", "15", "16","17","18"};
  std::vector<String> sunrise = {"06:42", "06:43", "06:45", "06:48", "06:48","06:50","06:51"};
  std::vector<String> alot = {"00:00", "05:03", "05:05", "05:08", "05:08","05:09","05:11"};
  std::vector<String> sunset = {"19:12", "19:11", "19:09", "19:08", "19:08","19:07","19:05"};
  std::vector<String> shma = {"08:15", "08:16", "08:18", "08:20", "08:21","08:22","08:25"};
  std::vector<String> hatzot = {"12:23", "12:24", "12:25", "12:27", "12:29","12:30","12:31"};
  std::vector<String> plag = {"16:20", "16:21", "16:21", "16:21", "16:22","16:23","16:25"};
  String shabat_start = "18:52";
  String shabat_end = "20:01";
  String shabat_tam = "20:40";
  String parasha = "אחרי מות - קדושים";


  UpdateCurrHebdate(curr_hebdate);
  UpdateCurrJuldate(curr_juldate);

  UpdateHebtime(hebtime);
  UpdateStandardTime(standardtime);

  UpdateCity(city);

  UpdateHebdates(hebdates);
  UpdateJuldates(juldates);

  UpdateSunrise(sunrise);
  UpdateSunset(sunset);
  UpdateAlot(alot);

  UpdatePlag(plag);
  UpdateShma(shma);
  UpdateHatzot(hatzot);

  UpdateShabat(shabat_start, shabat_end, shabat_tam);
  UpdateParasha(parasha);

  WriteString(StringObj("פרשת",195, 242, 5, 5, Rubik_Light14), m_fex);
}

void WeekViewScreen::TFTInitUIFrame(void)
{

  for(int i = 0; i < m_numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / m_numOfLines) * i;
    m_tft.drawLine(0,portion,TFT_WIDTH,portion, m_lineColor);
  }
  // create last horizontal line
  m_tft.drawLine(0,TFT_HEIGHT-1,TFT_WIDTH,TFT_HEIGHT-1, m_lineColor);

  // create two vertical numOfLines
  m_tft.drawLine(0,0,0,TFT_HEIGHT, m_lineColor);
  m_tft.drawLine(TFT_WIDTH-1, 0, TFT_WIDTH-1, TFT_HEIGHT, m_lineColor);

  // day and date seperator
  m_tft.drawLine(196, 40, 196, TFT_HEIGHT , m_lineColor);

  // shabat lines
  m_tft.drawLine(143, 280, 143 , 307 , m_lineColor);
  m_tft.drawLine(162, 280, 162 , 307 , m_lineColor);
  m_tft.drawLine(107, 307, 162 , 307 , m_lineColor);

  m_tft.drawLine(143, 240, 143 , 255 , m_lineColor);
  m_tft.drawLine(162, 240, 162 , 255 , m_lineColor);
  m_tft.drawLine(107, 255, 162 , 255 , m_lineColor);

  // first ח פ ש
  DrawColVertLine(37, m_tft);
  DrawColVertLine(51, m_tft);

  // second עש ז קש
  DrawColVertLine(87, m_tft);
  DrawColVertLine(107, m_tft);

  int days_lim = 194;
  int hebdate_lim = 145;
  int juldate_lim = 60;

  // jul date delimiter
  m_tft.drawLine(hebdate_lim, 0, hebdate_lim, 40, m_lineColor);

  // city name delimiter
  m_tft.drawLine(juldate_lim, 0, juldate_lim, 40, m_lineColor);

  // days in hebrew
  std::vector<String> days = {"א", "ב", "ג", "ד", "ה","ו", "ז"};
  WriteToCols(StringArrObj(days, TFT_WIDTH - 5, 7, 15 ,15,Rubik_Light28), m_fex);

  WriteString(StringObj("שעה", TFT_WIDTH - 2, 5, 10, 10, Rubik_Light10), m_fex);
  WriteString(StringObj("זמנית", TFT_WIDTH - 2, 15, 10, 10, Rubik_Light10), m_fex);
}
