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
  String days[DAYS_IN_WEEK];
  String curr_juldate[DATE_ELEMENTS];
  String curr_hebdate[DATE_ELEMENTS];
  String juldays[DAYS_IN_WEEK];
  String hebdays[DAYS_IN_WEEK];
  String sunrise[DAYS_IN_WEEK];
  String sunset[DAYS_IN_WEEK];
  String alot[DAYS_IN_WEEK];
}cal_data_ty;

void WriteString(const String &string, int32_t x, int32_t y, const uint8_t *font, int is_RTL);
void WriteToCols(String str_arr[], int32_t x, int32_t y, const uint8_t *font, int is_RTL);
void DrawColLine(int x);

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


void InitTFT(void)
{
  
  tft.init();
  int bg = TFT_DARKCYAN;
  int fg = TFT_SILVER;

  tft.setTextColor(fg, bg);
  tft.fillScreen(bg);

  TFTCreateUIFrame();

  // init struct
  cal.city = "ירושלים";
  String hebtime = "01:23";
  String standardtime = "20:46";
  String curr_hebdate[] = {"כה","ניסן","תשפד"};
  String curr_juldate[] = {"12","04","2024"};

  memcpy(cal.curr_hebdate, curr_hebdate, sizeof(String) * DATE_ELEMENTS);
  memcpy(cal.curr_juldate, curr_juldate, sizeof(String) * DATE_ELEMENTS);


  int days_lim = 194;
  int hebdate_lim = 145;
  int juldate_lim = 60;

  // heb date
  String hebdate = cal.curr_hebdate[0]+" "+cal.curr_hebdate[1]+" "+cal.curr_hebdate[2];
  WriteString("שעה", TFT_WIDTH - 2, 5, Rubik_Light10, RTL);
  WriteString("זמנית", TFT_WIDTH - 2, 15, Rubik_Light10, RTL);
  WriteString(hebtime, hebdate_lim + 2, 5, Rubik_Light26, LTR);
  WriteString(hebdate, TFT_WIDTH - 1, 25, Rubik_Light14 , RTL);

  // jul date
  fex.drawLine(hebdate_lim, 0, hebdate_lim, 40, TFT_LIGHTGREY);

  String juldate = cal.curr_juldate[0]+"/"+cal.curr_juldate[1]+"/"+cal.curr_juldate[2];
  WriteString(standardtime, juldate_lim + 10, 5, Rubik_Light26, LTR);
  WriteString(juldate, juldate_lim + 2, 25, Rubik_Light14 , LTR);

  // city name
  fex.drawLine(juldate_lim, 0, juldate_lim, 40, TFT_LIGHTGREY);
  WriteString(cal.city, juldate_lim - 2, 10, Rubik_Light20, RTL);


  String sunset_notation[] = {"ש", "ש", "ש", "ש", "ש","ש", "ש"};
  String alot_notation[] = {"עש", "עש", "עש", "עש", "עש","עש", "עש"};

  String days[] = {"א", "ב", "ג", "ד", "ה","ו", "ז"};
  String hebdays[] = {"כה", "כו", "כז", "כח", "כט","ל", "א"};
  String juldays[] = {"12", "13", "14", "15", "16","17","18"};
  String sunrise[] = {"00:00", "06:43", "06:45", "06:48", "06:48","06:50","06:51"};
  String alot[] = {"05:02", "05:03", "05:05", "05:08", "05:08","05:09","05:11"};
  String sunset[] = {"19:12", "19:11", "19:09", "19:08", "19:08","19:07","19:05"};
  String riseset_str[] = {"זריחה/שקיעה", "זריחה/שקיעה", "זריחה/שקיעה", "זריחה/שקיעה", "זריחה/שקיעה","זריחה/שקיעה","זריחה/שקיעה"};

  memcpy(cal.days, days, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.juldays, juldays, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.hebdays, hebdays, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.sunrise, sunrise, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.sunset, sunset, sizeof(String) * DAYS_IN_WEEK);
  memcpy(cal.alot, alot, sizeof(String) * DAYS_IN_WEEK);

  // columns:
  // day, hebday, julday
  WriteToCols(cal.days, TFT_WIDTH - 5, 7, Rubik_Light28, RTL);
  WriteToCols(cal.juldays, days_lim + 7, 20, Rubik_Light14, LTR);
  WriteToCols(cal.hebdays, days_lim + 22, 5, Rubik_Light14, RTL); 

  fex.drawLine(days_lim, 40, days_lim, TFT_HEIGHT , TFT_LIGHTGREY);

  UpdateSunrise(cal.sunrise);
  WriteToCols(cal.alot, 3, 5, Rubik_Light12, LTR);
  WriteToCols(cal.sunset, 3, 25, Rubik_Light12, LTR); 

  DrawColLine(37);

  WriteToCols(alot_notation, 55, 5,Rubik_Light12, RTL);
  WriteToCols(sunset_notation, 55, 25,Rubik_Light12, RTL);

  DrawColLine(60);

}

UpdateSunrise(String sunrises[])
{
  int yaxis = 15;
  String sunrise_notation[] = {"ז", "ז", "ז", "ז", "ז","ז", "ז"};
  WriteToCols(sunrise_notation, 55, yaxis,Rubik_Light12, RTL);
  WriteToCols(sunrises, 3, yaxis, Rubik_Light12, LTR); 
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