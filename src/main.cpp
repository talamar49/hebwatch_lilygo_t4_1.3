#include <Arduino.h>
#include <RTClib.h>

#include "Framework/Screen/IScreen.h"
#include "Framework/Screen/ScreenManager.h"
#include "Framework/Data/TopicServer.h"
#include "Framework/IO/Buttons/Button.h"
#include "Screens/WeekViewScreen.h"
#include "Screens/SettingsScreen.h"

TFT_eSPI g_tft = TFT_eSPI();
TopicServer g_topicServer;
RTC_DS3231 rtc;
ButtonHandler buttonHandler(38, 37, 39); // Pass your pin numbers


void setup() 
{
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL pins for LilyGO TTGO T4

  if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, setting the time!");
    // Set the time to a known value (e.g., Jan 1, 2023)
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }

  buttonHandler.Initialize(g_topicServer);

  g_topicServer.Subscribe("RightButtonTopic",[](){
    std::cout << "right" << std::endl;
  });
  g_topicServer.Subscribe("LeftButtonTopic",[](){
    std::cout << "LeftButtonTopic" << std::endl;
  });
  g_topicServer.Subscribe("MiddleButtonTopic",[](){
    std::cout << "MiddleButtonTopic" << std::endl;
  });
  g_tft.init();
}


void loop() 
{
  static WeekViewScreen weekViewScreen(g_tft, g_topicServer);
  static SettingsScreen settingsScreen(g_tft, g_topicServer);

  static ScreenManager screenManager(&weekViewScreen);
  
  buttonHandler.CheckIsPressed();


  if(screenManager.GetCurrentScreen() == &weekViewScreen)
  {
    weekViewScreen.UpdateStandardTime(String(rtc.now().minute()) + ":" + String(rtc.now().second()));
  }

  // weekViewScreen.Render();
}
