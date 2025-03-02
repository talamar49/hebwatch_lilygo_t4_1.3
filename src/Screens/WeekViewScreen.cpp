#include "Screens/WeekViewScreen.h"
#include <EEPROM.h>


WeekViewScreen::WeekViewScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                                : IScreen(topicServer_, bgColor_, textColor_, lineColor_, numOfLines_),
                                  m_tft(tft_),
                                  m_fex(&m_tft),
                                  m_juldate_sprite(&m_tft),
                                  m_julTime_sprite(&m_tft),
                                  m_julDayNMonth_sprite(&m_tft),
                                  m_julYear_sprite(&m_tft),
                                  m_standardTime_sprite(&m_tft),
                                  m_zmanim_sprite(&m_tft)
{

  m_currentCity = LoadCityFromEEPROM();

  m_cityIt = CITY_MAP.find(m_currentCity);
  if(m_cityIt == CITY_MAP.end()) { // If not found, start at beginning
      m_cityIt = CITY_MAP.begin();
  }

  m_topicServer.Subscribe<DateTime>("DateTimeTopic",[this](DateTime now){
    m_datetimeNow = now;
  });

  m_topicServer.Subscribe("LeftButtonLongPressTopic",[this](){
    LeftButtonLongPressHandler();
  });

  m_topicServer.Subscribe("RightButtonLongPressTopic",[this](){
    RightButtonLongPressHandler();
  });

  m_topicServer.Subscribe("LeftButtonShortPressTopic",[this](){
    LeftButtonShortPressHandler();
  });

  m_topicServer.Subscribe("RightButtonShortPressTopic",[this](){
    RightButtonShortPressHandler();
  });

  m_topicServer.Subscribe("MiddleButtonShortPressTopic",[this](){
    MiddleButtonShortPressHandler();
  });
}

void WeekViewScreen::Render()
{
  m_tft.setTextColor(m_textColor, m_bgColor);
  m_tft.fillScreen(m_bgColor);
    
  TFTInitUIFrame();
  TFTInitContent();

  m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", m_cityIt->second);
}

void WeekViewScreen::Loop()
{
  if (IsInCalib()) 
  {
    if(m_bIsCityCalib)
    {
      LoopLogic();
    }
    else if(m_bIsJulTimeAndDateCalib)
    {
      CalibLogic();
    }
  } 
  else 
  {
    LoopLogic();
  }
}

String WeekViewScreen::LoadCityFromEEPROM() 
{
    int len = EEPROM.read(EEPROM_CITY_ADDR);
    
    // Validate length
    if(len <= 0 || len > MAX_CITY_LENGTH) {
        return CITY_MAP.begin()->first; // Return default
    }
    
    String city;
    for(int i = 0; i < len; i++) {
        city += static_cast<char>(EEPROM.read(EEPROM_CITY_ADDR + 1 + i));
    }
    return city;
}

void WeekViewScreen::SaveCityToEEPROM(const String& city) {
    // Clear previous data
    for(int i = 0; i < MAX_CITY_LENGTH + 1; i++) {
        EEPROM.write(EEPROM_CITY_ADDR + i, 0);
    }
    
    // Write new data
    EEPROM.write(EEPROM_CITY_ADDR, city.length());
    for(int i = 0; i < city.length(); i++) {
        EEPROM.write(EEPROM_CITY_ADDR + 1 + i, city[i]);
    }
    
    EEPROM.commit();
}

void WeekViewScreen::CalibLogic()
{
  // Determine background colors for highlighted fields
  uint32_t hourBg = (m_timeAndDateCalibIndex == 1) ? m_calibBgColor : 0;
  uint32_t minuteBg = (m_timeAndDateCalibIndex == 2) ? m_calibBgColor : 0;
  uint32_t dayBg = (m_timeAndDateCalibIndex == 3) ? m_calibBgColor : 0;
  uint32_t monthBg = (m_timeAndDateCalibIndex == 4) ? m_calibBgColor : 0;
  uint32_t yearBg = (m_timeAndDateCalibIndex == 5) ? m_calibBgColor : 0;

  // Update display with temporary DateTime and highlighted fields
  UpdateStandardTime({ToString(m_tempDateTime.hour()), ToString(m_tempDateTime.minute())}, hourBg, minuteBg);
  UpdateCurrJuldate({ToString(m_tempDateTime.day()), ToString(m_tempDateTime.month()), ToString(m_tempDateTime.year())}, dayBg, monthBg, yearBg);
}

void WeekViewScreen::LoopLogic()
{
  UpdateStandardTime({ToString(m_datetimeNow.hour()), ToString(m_datetimeNow.minute())}, m_bgColor, m_bgColor);
  UpdateCurrJuldate({ToString(m_datetimeNow.day()), ToString(m_datetimeNow.month()), ToString(m_datetimeNow.year())}, m_bgColor, m_bgColor, m_bgColor);
}

void WeekViewScreen::MiddleButtonShortPressHandler() {
  if (m_bIsCityCalib) 
  {
    m_bIsCityCalib = false;
    m_currentCity = m_cityIt->first;
    SaveCityToEEPROM(m_currentCity); // Save to EEPROM
    UpdateCity();

    CityCoord coords = m_cityIt->second;
    m_topicServer.Publish<CityCoord>("SelectedCityCoordsTopic", coords);
  }
  else if(m_bIsJulTimeAndDateCalib) 
  {
    m_timeAndDateCalibIndex++;
    if (m_timeAndDateCalibIndex > 5) 
    { // Exit after adjusting year
      m_bIsJulTimeAndDateCalib = false;
      m_timeAndDateCalibIndex = 0;
      m_datetimeNow = m_tempDateTime; // Save the adjusted time
      // Optionally notify the system of the new time:
      m_topicServer.Publish<DateTime>("AdjustDateTimeTopic", m_datetimeNow);
    }
  }
}

void WeekViewScreen::MiddleButtonLongPressHandler()
{

}

void WeekViewScreen::RightButtonShortPressHandler() {
  if (IsInCalib()) 
  {
    if(m_bIsCityCalib)
    {
      if(++m_cityIt == CITY_MAP.end()) 
      {
        m_cityIt = CITY_MAP.begin();
      }
      m_currentCity = m_cityIt->first;
      UpdateCity(m_calibBgColor);
    }
    else if(m_bIsJulTimeAndDateCalib)
    {
      switch (m_timeAndDateCalibIndex) {
        case 1: // Increment hour
          m_tempDateTime = m_tempDateTime + TimeSpan(0, 1, 0, 0);
          break;
        case 2: // Increment minute
          m_tempDateTime = m_tempDateTime + TimeSpan(0, 0, 1, 0);
          break;
        case 3: // Increment day
          m_tempDateTime = m_tempDateTime + TimeSpan(1, 0, 0, 0);
          break;
        case 4: // Increment month
          AdjustMonth(1);
          break;
        case 5: // Increment year
          AdjustYear(1);
          break;
      }
    }
  }
}
void WeekViewScreen::RightButtonLongPressHandler() {
  if (!IsInCalib()) {
    m_bIsJulTimeAndDateCalib = true;
    m_tempDateTime = m_datetimeNow; // Initialize with current time
    m_timeAndDateCalibIndex = 1;    // Start with hour
  }
}

void WeekViewScreen::LeftButtonShortPressHandler() {
  if (IsInCalib()) 
  {
    if(m_bIsCityCalib)
    {
      // Move to previous city
      if(m_cityIt == CITY_MAP.begin()) {
          m_cityIt = CITY_MAP.end();
      }
      --m_cityIt;

      m_currentCity = m_cityIt->first;
      UpdateCity(m_calibBgColor);
    }
    else if(m_bIsJulTimeAndDateCalib)
    {
      switch (m_timeAndDateCalibIndex) {
        case 1: // Decrement hour
          m_tempDateTime = m_tempDateTime + TimeSpan(0, -1, 0, 0);
          break;
        case 2: // Decrement minute
          m_tempDateTime = m_tempDateTime + TimeSpan(0, 0, -1, 0);
          break;
        case 3: // Decrement day
          m_tempDateTime = m_tempDateTime + TimeSpan(-1, 0, 0, 0);
          break;
        case 4: // Decrement month
          AdjustMonth(-1);
          break;
        case 5: // Decrement year
          AdjustYear(-1);
          break;
      }
    }
  }
}

void WeekViewScreen::LeftButtonLongPressHandler()
{
  if (!IsInCalib()) 
  {
    m_bIsCityCalib = true;
    m_cityIt = CITY_MAP.find(m_currentCity);
    if(m_cityIt == CITY_MAP.end()) { // If not found, start at beginning
        m_cityIt = CITY_MAP.begin();
    }
    m_currentCity = m_cityIt->first;
    UpdateCity(m_calibBgColor);
  }
}

bool WeekViewScreen::IsInCalib()
{
  return m_bIsJulTimeAndDateCalib || m_bIsCityCalib;
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


void WeekViewScreen::UpdateStandardTime(std::vector<String> standardtime_, uint32_t hourBg, uint32_t minuteBg) 
{
  WriteSpriteString(StringObj(standardtime_[0], 70, 5, 32, 18, Rubik_Light26), m_julTime_sprite, hourBg);
  WriteSpriteString(StringObj(standardtime_[1], 106, 5, 32, 18, Rubik_Light26), m_julTime_sprite, minuteBg);
}

void WeekViewScreen::UpdateCity(uint32_t cityBg)
{
  int spaceIndex = m_currentCity.indexOf(' '); // Check for space in the string
  
  if (spaceIndex != -1) { // If space exists
    // Split into two parts
    String firstPart = m_currentCity.substring(0, spaceIndex);
    String secondPart = m_currentCity.substring(spaceIndex + 1);

    // First line (original position)
    WriteString(StringObj(firstPart, 58, 3, 57, 35, Rubik_Light20), m_fex, cityBg);
    
    // Second line (one row below - adjust Y position based on font height)
    WriteString(StringObj(secondPart, 58, 18, 0, 0, Rubik_Light20), m_fex, cityBg);
  } else { 
    // No space - original single-line implementation
    WriteString(StringObj(m_currentCity, 58, 3, 57, 35, Rubik_Light20), m_fex, cityBg);
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

void WeekViewScreen::AdjustMonth(int delta) 
{
    uint16_t year = m_tempDateTime.year();
    uint8_t month = m_tempDateTime.month() + delta;
    uint8_t day = m_tempDateTime.day();

    // Handle overflow/underflow
    if (delta > 0) {
    while (month > 12) { month -= 12; year++; }
    } else {
    while (month < 1) { month += 12; year--; }
    }

    // Clamp day to the new month's maximum
    uint8_t maxDay = DaysInGivenMonth(year, month);
    if (day > maxDay) day = maxDay;

    m_tempDateTime = DateTime(year, month, day, m_tempDateTime.hour(), m_tempDateTime.minute(), m_tempDateTime.second());
}

void WeekViewScreen::AdjustYear(int delta) 
{
    uint16_t year = m_tempDateTime.year() + delta;
    uint8_t month = m_tempDateTime.month();
    uint8_t day = m_tempDateTime.day();

    // Clamp day to the new year's February
    uint8_t maxDay = DaysInGivenMonth(year, month);
    if (day > maxDay) day = maxDay;

    m_tempDateTime = DateTime(year, month, day, m_tempDateTime.hour(), m_tempDateTime.minute(), m_tempDateTime.second());
}

// Include DaysInGivenMonth and IsLeapYear from earlier if not already present
uint8_t WeekViewScreen::DaysInGivenMonth(uint16_t year, uint8_t month) 
{
    static const uint8_t days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (month == 2 && IsLeapYear(year)) ? 29 : days[month - 1];
}

bool WeekViewScreen::IsLeapYear(uint16_t year) 
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}