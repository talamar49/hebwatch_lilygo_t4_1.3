#include <Arduino.h>

#include "Screen/IScreen.h"
#include "Screen/ScreenManager.h"
#include "Screen/Screens/WeekViewScreen.h"
#include "Screen/Screens/SettingsScreen.h"

TFT_eSPI g_tft = TFT_eSPI();

void setup() 
{
  Serial.begin(115200);

  g_tft.init();
}


void loop() 
{
  static int counter = 0;
  static bool b = true;
  static WeekViewScreen weekViewScreen(g_tft);
  static SettingsScreen settingsScreen(g_tft);

  static ScreenManager screenManager(&weekViewScreen);

  counter = ++counter % 6;
  weekViewScreen.UpdateHebtime(String(counter));
  if (counter == 5)
  {
    if(b)
    {
      screenManager.SetScreen(&settingsScreen);
      screenManager.Render();
      b = false;
    }else
    {
      screenManager.SetScreen(&weekViewScreen);
      screenManager.Render();
      b = true;
    }
  }

  delay(1000);
  // weekViewScreen.Render();
}
