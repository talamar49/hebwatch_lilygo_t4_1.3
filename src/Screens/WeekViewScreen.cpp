// WeekViewScreen.cpp
#include "Screens/WeekViewScreen.h"

WeekViewScreen::WeekViewScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
    : IScreen(topicServer_, bgColor_, textColor_, lineColor_, numOfLines_),
      m_tft(tft_),
      m_fex(&m_tft),
      m_juldate_sprite(&m_tft),
      m_julTime_sprite(&m_tft),
      m_julDayNMonth_sprite(&m_tft),
      m_julYear_sprite(&m_tft),
      m_standardTime_sprite(&m_tft),
      m_zmanim_sprite(&m_tft),
      m_delim_sprite(&m_tft),
      m_weekLogic(topicServer_)
{
  m_topicServer.Subscribe<CityCoord>("SelectedCityCoordsTopic", [this](CityCoord cityCoord) {
    uint32_t cityBg = 0; 
    
    if(m_weekLogic.IsInCityCalib())
    {
      cityBg = m_calibBgColor;
    }

    UpdateCity(cityBg);
  });
}

std::vector<String> WeekViewScreen::SplitStringBySpace(const String& input) {
  std::vector<String> result;
  int start = 0;
  int end = input.indexOf(' ');

  while (end != -1) {
      result.push_back(input.substring(start, end));
      start = end + 1;
      end = input.indexOf(' ', start);
  }

  // Add the last word
  result.push_back(input.substring(start));

  return result;
}
void WeekViewScreen::Render()
{
    m_tft.setTextColor(m_textColor, m_bgColor);
    m_tft.fillScreen(m_bgColor);
    TFTInitUIFrame();
    TFTInitContent();
    m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_weekLogic.GetCurrentCityCoord());
}

std::vector<DateTime> WeekViewScreen::GetWeekDatetimeFromCurrentDay(DateTime current)
{    
  std::vector<DateTime> week;

  // Get day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
  uint8_t currentDayOfWeek = current.dayOfTheWeek();  // 0-6

  // Calculate the date of Sunday (start of the week)
  DateTime startOfWeek = current - TimeSpan(currentDayOfWeek * 86400); // 86400 seconds in a day

  // Add 7 days to the vector starting from Sunday
  for (int i = 0; i < 7; ++i) {
      week.push_back(startOfWeek + TimeSpan(i * 86400));
  }

  return week;

}

void WeekViewScreen::Loop()
{
    m_weekLogic.OnLoop();
    
    m_hourBg = 0, m_minuteBg = 0, m_dayBg = 0, m_monthBg = 0, m_yearBg = 0;

    DateTime current = m_weekLogic.IsInDateTimeCalib() ? m_weekLogic.GetTempDateTime() : m_weekLogic.GetCurrentDateTime();
    
    std::vector<DateTime> weekDatetime = GetWeekDatetimeFromCurrentDay(current);

    ZmanimData zmanimData = m_zmanimLogic.GetZmanim(current);
    std::vector<ZmanimData> zmanimWeek = m_zmanimLogic.GetZmanimForRange(weekDatetime);
    
    UpdateCalibrationLogic();
    
    UpdateStandardTime({ToString(current.hour()), ToString(current.minute())}, m_hourBg, m_minuteBg, current.second());
    UpdateCurrJuldate({ToString(current.day()), ToString(current.month()), ToString(current.year())}, m_dayBg, m_monthBg, m_yearBg);
    
    if(zmanimData.hebrewDate != m_hebrewDate){
      m_hebrewDate = zmanimData.hebrewDate;
      UpdateCurrHebdate(SplitStringBySpace(m_hebrewDate));
    }
}

void WeekViewScreen::UpdateCalibrationLogic()
{
  
  if (m_weekLogic.IsInDateTimeCalib()) {
      switch (m_weekLogic.GetCalibIndex()) {
          case 1: m_hourBg = m_calibBgColor; break;
          case 2: m_minuteBg = m_calibBgColor; break;
          case 3: m_dayBg = m_calibBgColor; break;
          case 4: m_monthBg = m_calibBgColor; break;
          case 5: m_yearBg = m_calibBgColor; break;
      }
  }
}

void WeekViewScreen::UpdateCurrHebdate(std::vector<String> curr_hebdate_)
{
  String hebdate = curr_hebdate_[0]+" "+curr_hebdate_[1]+" "+curr_hebdate_[2];
  
  WriteString(StringObj(hebdate, TFT_WIDTH - 1, 25, 90, 15, Rubik_Light14), m_fex);
}

void WeekViewScreen::UpdateCurrJuldate(std::vector<String> curr_juldate_, uint32_t dayBg, uint32_t monthBg, uint32_t yearBg) {
  WriteSpriteString(StringObj(curr_juldate_[0], 62, 25, 18, 13, Rubik_Light14), m_julDayNMonth_sprite, dayBg);
  WriteSpriteString(StringObj(curr_juldate_[1], 86, 25, 18, 13, Rubik_Light14), m_julDayNMonth_sprite, monthBg);
  WriteSpriteString(StringObj(curr_juldate_[2], 109, 25, 36, 13, Rubik_Light14), m_julYear_sprite, yearBg);
}

void WeekViewScreen::UpdateHebtime(const String &hebtime_)
{
  WriteSpriteString(StringObj(hebtime_, 147, 5, 68, 18, Rubik_Light26), m_standardTime_sprite);
}


void WeekViewScreen::UpdateStandardTime(std::vector<String> standardtime_, uint32_t hourBg, uint32_t minuteBg, uint8_t currSeconds) 
{
  static uint8_t lastSecond = 0;
  static bool showColon = true;

  WriteSpriteString(StringObj(standardtime_[0], 70, 5, 32, 18, Rubik_Light26), m_julTime_sprite, hourBg);
  WriteSpriteString(StringObj(standardtime_[1], 106, 5, 32, 18, Rubik_Light26), m_julTime_sprite, minuteBg);
  
  if(currSeconds != lastSecond)
  {
    showColon = !showColon;
    String delim = showColon ? ":" : " ";
    WriteSpriteString(StringObj(delim, 101, 5, 5, 18, Rubik_Light26), m_delim_sprite);
    lastSecond = currSeconds;
  }
}

void WeekViewScreen::UpdateCity(uint32_t cityBg)
{
  String city = m_weekLogic.GetCurrentCity();
  int spaceIndex = city.indexOf(' '); // Check for space in the string

  if (spaceIndex != -1) {
    String firstPart = city.substring(0, spaceIndex);
    String secondPart = city.substring(spaceIndex + 1);

    WriteString(StringObj(firstPart, 58, 3, 57, 35, Rubik_Light20), m_fex, cityBg);
    WriteString(StringObj(secondPart, 58, 18, 0, 0, Rubik_Light20), m_fex, cityBg);
  } else {
    WriteString(StringObj(city, 58, 3, 57, 35, Rubik_Light20), m_fex, cityBg);
  }
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
  String city = "תל אביב";
  String hebtime = "00:00";
  std::vector<String> standardtime = {"00","00"};
  std::vector<String> curr_hebdate = {"כה","ניסן","תשגד"};
  std::vector<String> curr_juldate = {"00","00","0000"};
  std::vector<String> hebdates = {"כה", "כו", "כז", "כח", "כט","ל", "א"};
  std::vector<String> juldates = {"00", "00", "00", "00", "00","00","00"};
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

  UpdateCity();

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
  DrawColVerticalLine(37, m_tft);
  DrawColVerticalLine(51, m_tft);

  // second עש ז קש
  DrawColVerticalLine(87, m_tft);
  DrawColVerticalLine(107, m_tft);

  int days_lim = 194;
  int hebdate_lim = 145;
  int juldate_lim = 60;

  // jul date delimiter
  m_tft.drawLine(hebdate_lim, 0, hebdate_lim, 40, m_lineColor);

  // city name delimiter
  m_tft.drawLine(juldate_lim, 0, juldate_lim, 40, m_lineColor);

  // jul date delim
  WriteTFTString(StringObj("/", 80, 25, 0, 0, Rubik_Light14),m_tft);
  WriteTFTString(StringObj("/", 103, 25, 0, 0, Rubik_Light14),m_tft);

  // jul time delim
  WriteTFTString(StringObj(":", 101, 5, 0, 0, Rubik_Light26), m_tft);

  // days in hebrew
  std::vector<String> days = {"א", "ב", "ג", "ד", "ה","ו", "ז"};
  WriteToCols(StringArrObj(days, TFT_WIDTH - 5, 7, 15 ,15,Rubik_Light28), m_fex);

  WriteString(StringObj("שעה", TFT_WIDTH - 2, 5, 10, 10, Rubik_Light10), m_fex);
  WriteString(StringObj("זמנית", TFT_WIDTH - 2, 15, 10, 10, Rubik_Light10), m_fex);
}


