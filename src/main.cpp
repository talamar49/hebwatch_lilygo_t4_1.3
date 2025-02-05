#include <Arduino.h> // arduino functions
#include <TFT_eSPI.h> // spi function
#include <TFT_eFEX.h>              // Include the extension graphics functions library

#include <time.h> // all time related functions
#include <stdlib.h> // rand
#include <string.h>

#include "Rubik-Light10.h"
#include "Rubik-Light12.h"
#include "Rubik-Light14.h"
#include "Rubik-Light20.h"
#include "Rubik-Light26.h"
#include "Rubik-Light28.h"

#include <vector>
#define DAYS_IN_WEEK 7
#define DATE_ELEMENTS 3


// Button handler
void ButtonHandler(void);
void OnLeft(void);
void OnRight(void);
void OnSelect(void);

// UI
void TFTInitUIFrame(void);
void WriteSpriteString(const String &str, int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eSprite& tftSprite);
void WriteSpriteToCols(const String str_arr[], int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eSprite& tftSprite);
void WriteString(const String &string, int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eFEX& fex_);
void WriteToCols(const String str_arr[], int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eFEX& fex);
void DrawColVertLine(int x);

// all the function in setup
void InitSetup(void);
void InitTFT(void);

// Update UI
void TFTInitContent(void);
void UpdateHebdates(const String hebdates[]);
void UpdateJuldates(const String juldates[]);
void UpdateSunrise(const String sunrises[]);
void UpdateSunset(const String sunsets[]);
void UpdateAlot(const String alots[]);
void UpdatePlag(const String plags[]);
void UpdateShma(const String shmas[]);
void UpdateHatzot(const String hatzots[]);
void UpdateShabat(const String shabat_start, String shabat_end, String shabat_tam);
void UpdateCurrHebdate(const String curr_hebdate[]);
void UpdateCurrJuldate(const String curr_juldate[]);
void UpdateStandardTime(const String &standardtime);
void UpdateHebtime(const String &hebtime);
void UpdateCity(const String &city);

enum
{
  INIT_BUTTON,
  RIGHT_BUTTON,
  LEFT_BUTTON,
  OnSelect_BUTTON
};

TFT_eSPI tft = TFT_eSPI();
TFT_eFEX fex = TFT_eFEX(&tft);
TFT_eSprite zmanim_sprite = TFT_eSprite(&tft);
TFT_eSprite jultime_sprite = TFT_eSprite(&tft);
TFT_eSprite standardTime_sprite = TFT_eSprite(&tft);
TFT_eSprite juldate_sprite = TFT_eSprite(&tft);

const byte right_pin = 39;
const byte left_pin = 38;  
const byte middle_pin = 37;  
int button_state = INIT_BUTTON;

uint32_t bgColor = TFT_DARKCYAN;
uint32_t textColor = TFT_SILVER;
uint32_t lineColor = TFT_LIGHTGREY;

uint32_t numOfLines = 8;


void setup() 
{
  InitSetup();
  InitTFT();
}

void loop() 
{
  String cities[] = {"ץ","ת","צ","מ","ך","ו","ט","כ","ע","ר"};

  for(int i = 0; i < 10; i++)
  {
    String City = String("נהריה") + cities[i] + String("ת") ;
    String alot[] = {String(i) ,"2","4","1","2","4","3"};
    String date[] = {"0"+String(i) ,"12", "202"+String(i)};
    String standardTime = String(i)+String(i)+":"+String(i)+String(i);
    UpdateAlot(alot);
    UpdatePlag(alot);
    // UpdateCurrJuldate(date);
    UpdateStandardTime(standardTime);
    UpdateHebtime(standardTime);
    UpdateCity(City);
    delay(500);
  }
  // Serial.println("fhjd");
}

// ########## Function Decleration ######### //
void ButtonHandler(void)
{
  switch(button_state)
  {
    case LEFT_BUTTON:
      tft.println("LEFT");
      button_state = INIT_BUTTON;
      break;
      
    case OnSelect_BUTTON:
      tft.println("OnSelect");
      button_state = INIT_BUTTON;
      break;

    case RIGHT_BUTTON:
      tft.println("RIGHT");
      button_state = INIT_BUTTON;
      break;
  }
}

void OnLeft(void)
{
  button_state = LEFT_BUTTON;
}

void OnRight(void)
{
  button_state = RIGHT_BUTTON;
}

void OnSelect(void)
{
  button_state = OnSelect_BUTTON;
}


// ############ Setup Settings ############### //

void InitSetup(void)
{
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(right_pin), OnRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(middle_pin), OnSelect, FALLING);
  attachInterrupt(digitalPinToInterrupt(left_pin), OnLeft, FALLING);
}

void InitTFT(void)
{
  
  tft.init();

  tft.setTextColor(textColor, bgColor);
  tft.fillScreen(bgColor);

  TFTInitUIFrame();
  TFTInitContent();
}

void UpdateParasha(const String &parash)
{
  WriteString(parash, 195, 256, 86, 12, Rubik_Light12, fex);
}

void TFTInitContent(void)
{
    // init struct
  String city = "ירושלים";
  String hebtime = "01:23";
  String standardtime = "20:46";
  String curr_hebdate[] = {"כה","ניסן","תשגד"};
  String curr_juldate[] = {"12","04","2024"};
  String hebdates[] = {"כה", "כו", "כז", "כח", "כט","ל", "א"};
  String juldates[] = {"00", "13", "14", "15", "16","17","18"};
  String sunrise[] = {"06:42", "06:43", "06:45", "06:48", "06:48","06:50","06:51"};
  String alot[] = {"00:00", "05:03", "05:05", "05:08", "05:08","05:09","05:11"};
  String sunset[] = {"19:12", "19:11", "19:09", "19:08", "19:08","19:07","19:05"};
  String shma[] = {"08:15", "08:16", "08:18", "08:20", "08:21","08:22","08:25"};
  String hatzot[] = {"12:23", "12:24", "12:25", "12:27", "12:29","12:30","12:31"};
  String plag[] = {"16:20", "16:21", "16:21", "16:21", "16:22","16:23","16:25"};
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

  WriteString("פרשת",195, 242, 5, 5, Rubik_Light14, fex);
}

void UpdateCurrHebdate(const String curr_hebdate[])
{
  String hebdate = curr_hebdate[0]+" "+curr_hebdate[1]+" "+curr_hebdate[2];
  WriteString(hebdate, TFT_WIDTH - 1, 25, 90, 15, Rubik_Light14, fex);
}

void UpdateCurrJuldate(const String curr_juldate[])
{
  String juldate = curr_juldate[0]+"/"+curr_juldate[1]+"/"+curr_juldate[2];
  WriteSpriteString(juldate, 62, 25, 77, 13, Rubik_Light14, jultime_sprite);
}

void UpdateHebtime(const String &hebtime)
{
  WriteSpriteString(hebtime, 147, 5, 68, 18, Rubik_Light26, standardTime_sprite);
}

void UpdateStandardTime(const String &standardtime)
{
  WriteSpriteString(standardtime, 70, 5, 68, 18, Rubik_Light26, standardTime_sprite);
}

void UpdateCity(const String &city)
{
  WriteString(city, 58, 10, 55, 20, Rubik_Light20, fex);
}

void UpdateHebdates(const String hebdates[])
{
  WriteToCols(hebdates, 216, 5, 18, 13, Rubik_Light14, fex); 
}

void UpdateJuldates(const String juldates[])
{
  // WriteToCols(juldates, 201, 20, 19, 13, Rubik_Light14, LTR);
  WriteSpriteToCols(juldates, 201, 20, 19, 13, Rubik_Light14, juldate_sprite);
}

void UpdateShabat(const String shabat_start, String shabat_end, String shabat_tam)
{
  WriteSpriteString(shabat_start, 110, 243, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteSpriteString(shabat_end, 110, 283, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteSpriteString(shabat_tam, 110, 295, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteString("כש", 160, 243, 5, 5, Rubik_Light12, fex);
  WriteString("צש", 160, 283, 5, 5, Rubik_Light12, fex);
  WriteString("רת", 160, 295, 5, 5, Rubik_Light12, fex);
}

void UpdateAlot(const String alots[])
{
  int yaxis = 3;
  static String alot_notation[] = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  WriteSpriteToCols(alots, 54, yaxis, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteToCols(alot_notation, 105, yaxis, 5, 5, Rubik_Light12, fex);
}

void UpdateSunrise(const String sunrises[])
{
  int yaxis = 15;
  static String sunrise_notation[] = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};

  WriteSpriteToCols(sunrises, 54, yaxis, 32, 10, Rubik_Light12, zmanim_sprite); 
  WriteToCols(sunrise_notation, 105, yaxis, 5, 5, Rubik_Light12, fex);
}

void UpdateShma(const String shmas[])
{
  int yaxis = 27;
  static String shma_notation[] = {"קש", "קש", "קש", "קש", "קש","קש", "קש"};

  WriteSpriteToCols(shmas, 54, yaxis, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteToCols(shma_notation, 105, yaxis, 5, 5, Rubik_Light12, fex);
}

void UpdateHatzot(const String hatzots[])
{
  int yaxis = 3;
  static String hatzot_notation[] = {"ח", "ח", "ח", "ח", "ח","ח", "ח"};

  WriteSpriteToCols(hatzots, 3, yaxis, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteToCols(hatzot_notation, 48, yaxis, 5, 5, Rubik_Light12, fex);
}

void UpdatePlag(const String plags[])
{
  int yaxis = 15;
  static String plag_notation[] = {"פ", "פ", "פ", "פ", "פ","פ", "פ"};

  WriteSpriteToCols(plags, 3, yaxis, 32, 10, Rubik_Light12, zmanim_sprite);
  WriteToCols(plag_notation, 48, yaxis, 5, 5, Rubik_Light12, fex);
}

void UpdateSunset(const String sunsets[])
{
  int yaxis = 27;
  static String sunset_notation[] = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};

  WriteSpriteToCols(sunsets, 3, yaxis, 32, 10, Rubik_Light12, zmanim_sprite); 
  WriteToCols(sunset_notation, 48, yaxis, 5, 5, Rubik_Light12, fex);
}

void DrawColVertLine(int x)
{
  fex.drawLine(x, 40, x, TFT_HEIGHT ,lineColor);
}

void TFTInitUIFrame(void)
{

  for(int i = 0; i < numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / numOfLines) * i;
    tft.drawLine(0,portion,TFT_WIDTH,portion, lineColor);
  }
  // create last horizontal line
  tft.drawLine(0,TFT_HEIGHT-1,TFT_WIDTH,TFT_HEIGHT-1, lineColor);

  // create two vertical numOfLines
  tft.drawLine(0,0,0,TFT_HEIGHT, lineColor);
  tft.drawLine(TFT_WIDTH-1, 0, TFT_WIDTH-1, TFT_HEIGHT, lineColor);

  // day and date seperator
  tft.drawLine(196, 40, 196, TFT_HEIGHT , lineColor);

  // shabat lines
  tft.drawLine(143, 280, 143 , 307 , lineColor);
  tft.drawLine(162, 280, 162 , 307 , lineColor);
  tft.drawLine(107, 307, 162 , 307 , lineColor);

  tft.drawLine(143, 240, 143 , 255 , lineColor);
  tft.drawLine(162, 240, 162 , 255 , lineColor);
  tft.drawLine(107, 255, 162 , 255 , lineColor);

  // first ח פ ש
  DrawColVertLine(37);
  DrawColVertLine(51);

  // second עש ז קש
  DrawColVertLine(87);
  DrawColVertLine(107);

  int days_lim = 194;
  int hebdate_lim = 145;
  int juldate_lim = 60;

  // jul date delimiter
  fex.drawLine(hebdate_lim, 0, hebdate_lim, 40, lineColor);

  // city name delimiter
  fex.drawLine(juldate_lim, 0, juldate_lim, 40, lineColor);

  // days in hebrew
  String days[] = {"א", "ב", "ג", "ד", "ה","ו", "ז"};
  WriteToCols(days, TFT_WIDTH - 5, 7, 15 ,15,Rubik_Light28, fex);

  WriteString("שעה", TFT_WIDTH - 2, 5, 10, 10, Rubik_Light10, fex);
  WriteString("זמנית", TFT_WIDTH - 2, 15, 10, 10, Rubik_Light10, fex);
}

void WriteToCols(const String str_arr[], int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eFEX& fex)
{
  for(int i = 1; i < numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / numOfLines) * (i);

    WriteString(str_arr[i-1], x, y + portion, width, height, font, fex);
  }
}


void WriteString(const String &str, int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eFEX& fex_)
{
  fex_.GetESpi()->loadFont(font);
  
  fex_.fillRect(x - width, y, width, height, bgColor); // renderer
  fex_.setCursorRTL(x, y);
  fex_.drawStringRTL(str);

  fex_.GetESpi()->unloadFont();
}

void WriteSpriteToCols(const String str_arr[], int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t *font, TFT_eSprite& tftSprite)
{
  for(int i = 1; i < numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / numOfLines) * (i);

    WriteSpriteString(str_arr[i-1], x, y + portion, width, height, font, tftSprite);
  }
}

void WriteSpriteString(const String &str, int32_t x, int32_t y, int32_t width, int32_t height, const uint8_t* font, TFT_eSprite& tftSprite)
{
  tftSprite.loadFont(font);

  tftSprite.createSprite(width, height);
  tftSprite.setTextColor(textColor, bgColor);
  tftSprite.fillScreen(bgColor);
  
  tftSprite.drawString(str, 0, 0);
  tftSprite.pushSprite(x, y);

  tftSprite.unloadFont();
}


































// tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextSize(3);
//   tft.drawRect(0,0,319,239,lineColor);
//   tft.drawLine(0,100,319,100,lineColor);
//   tft.drawLine(0,160,319,160,lineColor);
//   tft.drawLine(160,160,160,239,lineColor);
//   tft.drawLine(160,200,339,200,lineColor); //x1,y1,x2,y2
//   tft.setTextColor(lineColor,TFT_BLACK);
//   tft.drawString("23",40,175,2);//Temperature
//   tft.setTextColor(TFT_YELLOW,TFT_BLACK);
//   tft.setTextColor(TFT_WHITE,TFT_BLACK);
//   tft.drawString("00",250,16,2);// Seconds Digits x=250
//   tft.setTextColor(TFT_GREEN,TFT_BLACK);
//   tft.drawString("Monday",92,104,2);// x = 100
// //  tft.setTextColor(TFT_BLACK,TFT_BLACK);
// //  tft.drawString("38",155,16,4);// Minutes Digits x=155
//  // tft.drawString("12",50,16,4);// Hour Digits x = 50
//  tft.setTextColor(TFT_WHITE,TFT_BLACK);
//  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
//  tft.drawString("01",221,207,1);