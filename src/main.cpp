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

#define DAYS_IN_WEEK 7
#define DATE_ELEMENTS 3

// handle button press
void ButtonHandler(void);
// ISR for left button
void ChooseLeft(void);
// ISR for right button
void ChooseRight(void);
// ISR for middle button
void Select(void);

// UI
void TFTCreateUIFrame(void);

// all the function in setup
void InitSetup(void);
void InitTFT(void);
void InitSprite(void);

enum
{
  LTR = 0,
  RTL = 1
};

typedef struct cal_data_ty
{
  String city;
  String curr_juldate[DATE_ELEMENTS];
  String curr_hebdate[DATE_ELEMENTS];
  String juldates[DAYS_IN_WEEK];
  String hebdates[DAYS_IN_WEEK];
  String sunrise[DAYS_IN_WEEK];
  String sunset[DAYS_IN_WEEK];
  String alot[DAYS_IN_WEEK];
  String shma[DAYS_IN_WEEK];
  String hatzot[DAYS_IN_WEEK];
  String plag[DAYS_IN_WEEK];
  String shabat_start;
  String shabat_end;
  String shabat_tam;
}cal_data_ty;

void WriteString(const String &string, int32_t x, int32_t y, const uint8_t *font, int is_RTL);
void WriteToCols(String str_arr[], int32_t x, int32_t y, const uint8_t *font, int is_RTL);
void DrawColLine(int x);

void UpdateHebdates(String hebdates[]);
void UpdateJuldates(String juldates[]);
void UpdateSunrise(String sunrises[]);
void UpdateSunset(String sunsets[]);
void UpdateAlot(String alots[]);
void UpdatePlag(String plags[]);
void UpdateShma(String shmas[]);
void UpdateHatzot(String hatzots[]);
void UpdateShabat(String shabat_start, String shabat_end, String shabat_tam);



enum
{
  INIT_BUTTON,
  RIGHT_BUTTON,
  LEFT_BUTTON,
  SELECT_BUTTON
};

TFT_eSPI tft = TFT_eSPI();
TFT_eFEX fex = TFT_eFEX(&tft);
cal_data_ty cal;

const byte right_pin = 39;
const byte left_pin = 38;  
const byte middle_pin = 37;  


void setup() 
{
  InitSetup();
  InitTFT();
}

int button_state = INIT_BUTTON;

void loop() 
{

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
      
    case SELECT_BUTTON:
      tft.println("SELECT");
      button_state = INIT_BUTTON;
      break;

    case RIGHT_BUTTON:
      tft.println("RIGHT");
      button_state = INIT_BUTTON;
      break;
  }
}

void ChooseLeft(void)
{
  button_state = LEFT_BUTTON;
}

void ChooseRight(void)
{
  button_state = RIGHT_BUTTON;
}

void Select(void)
{
  button_state = SELECT_BUTTON;
}


// ############ Setup Settings ############### //

void InitSetup(void)
{
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(right_pin), ChooseRight, FALLING);
  attachInterrupt(digitalPinToInterrupt(middle_pin), Select, FALLING);
  attachInterrupt(digitalPinToInterrupt(left_pin), ChooseLeft, FALLING);
}

void UpdateCurrHebdate(String curr_hebdate[])
{
  String hebdate = curr_hebdate[0]+" "+curr_hebdate[1]+" "+curr_hebdate[2];
  WriteString(hebdate, TFT_WIDTH - 1, 25, Rubik_Light14 , RTL);
}

void UpdateCurrJuldate(String curr_juldate[])
{
  String juldate = curr_juldate[0]+"/"+curr_juldate[1]+"/"+curr_juldate[2];
  WriteString(juldate, 62, 25, Rubik_Light14 , LTR);
}

void UpdateHebtime(const String &hebtime)
{
  WriteString(hebtime, 147, 5, Rubik_Light26, LTR);
}

void UpdateStandardTime(const String &standardtime)
{
  WriteString(standardtime, 70, 5, Rubik_Light26, LTR);
}

void UpdateCity(const String &city)
{
  WriteString(city, 58, 10, Rubik_Light20, RTL);
}

void InitTFT(void)
{
  
  tft.init();

  tft.setTextColor(TFT_SILVER, TFT_DARKCYAN);
  tft.fillScreen(TFT_DARKCYAN);

  TFTCreateUIFrame();

  // init struct
  cal.city = "ירושלים";
  String hebtime = "01:23";
  String standardtime = "20:46";
  String curr_hebdate[] = {"כה","ניסן","תשפד"};
  String curr_juldate[] = {"12","04","2024"};
  String hebdates[] = {"כה", "כו", "כז", "כח", "כט","ל", "א"};
  String juldates[] = {"12", "13", "14", "15", "16","17","18"};
  String sunrise[] = {"06:42", "06:43", "06:45", "06:48", "06:48","06:50","06:51"};
  String alot[] = {"00:00", "05:03", "05:05", "05:08", "05:08","05:09","05:11"};
  String sunset[] = {"19:12", "19:11", "19:09", "19:08", "19:08","19:07","19:05"};
  String shma[] = {"08:15", "08:16", "08:18", "08:20", "08:21","08:22","08:25"};
  String hatzot[] = {"12:23", "12:24", "12:25", "12:27", "12:29","12:30","12:31"};
  String plag[] = {"16:20", "16:21", "16:21", "16:21", "16:22","16:23","16:25"};
  cal.shabat_start = "18:52";
  cal.shabat_end = "20:01";
  cal.shabat_tam = "20:40";

  memcpy(cal.curr_hebdate, curr_hebdate, sizeof(String) * DATE_ELEMENTS);
  memcpy(cal.curr_juldate, curr_juldate, sizeof(String) * DATE_ELEMENTS);
  memcpy(cal.juldates, juldates, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.hebdates, hebdates, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.sunrise, sunrise, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.sunset, sunset, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.alot, alot, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.shma, shma, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.hatzot, hatzot, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.plag, plag, sizeof(String) * DAYS_IN_WEEK);

  UpdateCurrHebdate(cal.curr_hebdate);
  UpdateCurrJuldate(cal.curr_juldate);

  UpdateHebtime(hebtime);
  UpdateStandardTime(standardtime);

  UpdateCity(cal.city);

  UpdateHebdates(cal.hebdates);
  UpdateJuldates(cal.juldates);

  UpdateSunrise(cal.sunrise);
  UpdateSunset(cal.sunset);
  UpdateAlot(cal.alot);

  UpdatePlag(cal.plag);
  UpdateShma(cal.shma);
  UpdateHatzot(cal.hatzot);

  UpdateShabat(cal.shabat_start, cal.shabat_end, cal.shabat_tam);
}

void UpdateHebdates(String hebdates[])
{
  WriteToCols(hebdates, 216, 5, Rubik_Light14, RTL); 
}

void UpdateJuldates(String juldates[])
{
  WriteToCols(juldates, 201, 20, Rubik_Light14, LTR);
}

void UpdateShabat(String shabat_start, String shabat_end, String shabat_tam)
{
  WriteString(shabat_start, 110, 243, Rubik_Light12, LTR);
  WriteString(shabat_end, 110, 283, Rubik_Light12, LTR);
  WriteString(shabat_tam, 110, 295, Rubik_Light12, LTR);
  WriteString("כש", 160, 243, Rubik_Light12, RTL);
  WriteString("צש", 160, 283, Rubik_Light12, RTL);
  WriteString("רת", 160, 295, Rubik_Light12, RTL);
}

void UpdateAlot(String alots[])
{
  int yaxis = 3;
  static String alot_notation[] = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  WriteToCols(alots, 54, yaxis, Rubik_Light12, LTR);
  WriteToCols(alot_notation, 105, yaxis,Rubik_Light12, RTL);
}

void UpdateSunrise(String sunrises[])
{
  int yaxis = 15;
  static String sunrise_notation[] = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};

  WriteToCols(sunrises, 54, yaxis, Rubik_Light12, LTR); 
  WriteToCols(sunrise_notation, 105, yaxis,Rubik_Light12, RTL);
}

void UpdateShma(String shmas[])
{
  int yaxis = 27;
  static String shma_notation[] = {"קש", "קש", "קש", "קש", "קש","קש", "קש"};

  WriteToCols(shmas, 54, yaxis, Rubik_Light12, LTR);
  WriteToCols(shma_notation, 105, yaxis,Rubik_Light12, RTL);
}

void UpdateHatzot(String hatzots[])
{
  int yaxis = 3;
  static String hatzot_notation[] = {"ח", "ח", "ח", "ח", "ח","ח", "ח"};

  WriteToCols(hatzots, 3, yaxis, Rubik_Light12, LTR);
  WriteToCols(hatzot_notation, 48, yaxis,Rubik_Light12, RTL);
}

void UpdatePlag(String plags[])
{
  int yaxis = 15;
  static String plag_notation[] = {"פ", "פ", "פ", "פ", "פ","פ", "פ"};

  WriteToCols(plags, 3, yaxis, Rubik_Light12, LTR);
  WriteToCols(plag_notation, 48, yaxis,Rubik_Light12, RTL);
}

void UpdateSunset(String sunsets[])
{
  int yaxis = 27;
  static String sunset_notation[] = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};

  WriteToCols(sunsets, 3, yaxis, Rubik_Light12, LTR); 
  WriteToCols(sunset_notation, 48, yaxis,Rubik_Light12, RTL);
}


void DrawColLine(int x)
{
  fex.drawLine(x, 40, x, TFT_HEIGHT , TFT_LIGHTGREY);
}

void TFTCreateUIFrame(void)
{
  int lines = 8;
  int lines_color = TFT_LIGHTGREY;

  for(int i = 0; i < lines; ++i)
  {
    int portion = (TFT_HEIGHT / lines) * i;
    tft.drawLine(0,portion,TFT_WIDTH,portion,lines_color);
  }
  // create last horizontal line
  tft.drawLine(0,TFT_HEIGHT-1,TFT_WIDTH,TFT_HEIGHT-1,lines_color);

  // create two vertical lines
  tft.drawLine(0,0,0,TFT_HEIGHT,lines_color);
  tft.drawLine(TFT_WIDTH-1, 0, TFT_WIDTH-1, TFT_HEIGHT,lines_color);

  // day and date seperator
  tft.drawLine(196, 40, 196, TFT_HEIGHT , TFT_LIGHTGREY);

  // shabat lines
  tft.drawLine(143, 280, 143 , 307 , TFT_LIGHTGREY);
  tft.drawLine(162, 280, 162 , 307 , TFT_LIGHTGREY);
  tft.drawLine(107, 307, 162 , 307 , TFT_LIGHTGREY);

  tft.drawLine(143, 240, 143 , 255 , TFT_LIGHTGREY);
  tft.drawLine(162, 240, 162 , 255 , TFT_LIGHTGREY);
  tft.drawLine(107, 255, 162 , 255 , TFT_LIGHTGREY);

  // first ח פ ש
  DrawColLine(37);
  DrawColLine(51);

  // second עש ז קש
  DrawColLine(87);
  DrawColLine(107);

  int days_lim = 194;
  int hebdate_lim = 145;
  int juldate_lim = 60;

  // jul date delimiter
  fex.drawLine(hebdate_lim, 0, hebdate_lim, 40, TFT_LIGHTGREY);

  // city name delimiter
  fex.drawLine(juldate_lim, 0, juldate_lim, 40, TFT_LIGHTGREY);

  // days in hebrew
  String days[] = {"א", "ב", "ג", "ד", "ה","ו", "ז"};
  WriteToCols(days, TFT_WIDTH - 5, 7, Rubik_Light28, RTL);

  WriteString("שעה", TFT_WIDTH - 2, 5, Rubik_Light10, RTL);
  WriteString("זמנית", TFT_WIDTH - 2, 15, Rubik_Light10, RTL);
}

void WriteToCols(String str_arr[], int32_t x, int32_t y, const uint8_t *font, int is_RTL)
{
  tft.loadFont(font);

  int i = 1;
  int lines = 8;

  for(; i < lines; ++i)
  {
    int portion = (TFT_HEIGHT / lines) * (i);

    if(is_RTL)
    {
    fex.setCursorRTL(x ,portion + y);
    fex.drawStringRTL(str_arr[i-1]);
    }else{
      tft.drawString(str_arr[i-1],x, portion + y);
    }
  }

  tft.unloadFont();
}

void WriteString(const String &string, int32_t x, int32_t y, const uint8_t *font, int is_RTL)
{
  tft.loadFont(font);

  if(is_RTL)
  {
    fex.setCursorRTL(x, y);
    fex.drawStringRTL(string);
  }else{
    tft.drawString(string, x, y);
  }

  tft.unloadFont();
}

































// tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextSize(3);
//   tft.drawRect(0,0,319,239,lines_color);
//   tft.drawLine(0,100,319,100,lines_color);
//   tft.drawLine(0,160,319,160,lines_color);
//   tft.drawLine(160,160,160,239,lines_color);
//   tft.drawLine(160,200,339,200,lines_color); //x1,y1,x2,y2
//   tft.setTextColor(lines_color,TFT_BLACK);
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