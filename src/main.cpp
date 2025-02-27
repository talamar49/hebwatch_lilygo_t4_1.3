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

  ButtonInitialize(g_topicServer);

  g_topicServer.Subscribe("RightButtonShortPressTopic",[](){
    std::cout << "RightButtonShortPressTopic" << std::endl;
  });
  g_topicServer.Subscribe("LeftButtonShortPressTopic",[](){
    std::cout << "LeftButtonShortPressTopic" << std::endl;
  });
  g_topicServer.Subscribe("MiddleButtonShortPressTopic",[](){
    std::cout << "MiddleButtonShortPressTopic" << std::endl;
  });
  g_topicServer.Subscribe("LeftButtonLongPressTopic",[](){
    std::cout << "LeftButtonLongPressTopic" << std::endl;
  });
  g_topicServer.Subscribe("MiddleButtonLongPressTopic",[](){
    std::cout << "MiddleButtonLongPressTopic" << std::endl;
  });
  g_topicServer.Subscribe("RightButtonLongPressTopic",[](){
    std::cout << "RightButtonLongPressTopic" << std::endl;
  });
  g_tft.init();
}


void loop() 
{
  static WeekViewScreen weekViewScreen(g_tft, g_topicServer);
  static SettingsScreen settingsScreen(g_tft, g_topicServer);
  static ScreenManager screenManager(&settingsScreen);
  
  g_topicServer.Publish<DateTime>("DateTimeTopic",rtc.now());
  ButtonCheckIsPressed();


  screenManager.Loop();
  delay(50);
}

