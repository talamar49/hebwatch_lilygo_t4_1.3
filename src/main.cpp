#include <Arduino.h>
#include <RTClib.h>
#include <EEPROM.h>

#include "Framework/Screen/IScreen.h"
#include "Framework/Screen/ScreenManager.h"
#include "Framework/Data/TopicServer.h"
#include "Framework/IO/RTC/RTCHelper.h"
#include "Framework/IO/Buttons/Button.h"
#include "Screens/WeekViewScreen.h"
#include "Screens/SettingsScreen.h"
#include "Utils/DataNeeded.h"

TFT_eSPI g_tft = TFT_eSPI();
TopicServer g_topicServer;

void IOHandler();

void setup() 
{
  Serial.begin(115200);
  EEPROM.begin(64); 
  Wire.begin(21, 22); // SDA, SCL pins for LilyGO TTGO T4
  
  RTCInitilize(g_topicServer);
  ButtonInitialize(g_topicServer);

  g_tft.init();
}

void loop() 
{
  static WeekViewScreen weekViewScreen(g_tft, g_topicServer);
  static ScreenManager screenManager(&weekViewScreen);

  IOHandler();
  
  screenManager.Loop();

  delay(50);
}

void IOHandler()
{
  ButtonCheckIsPressed();
  RTCPublishData();
}
