#include "Screens/SettingsScreen.h"

SettingsScreen::SettingsScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                                : IScreen(topicServer_, bgColor_, textColor_, lineColor_, numOfLines_),
                                  m_tft(tft_),
                                  m_fex(&m_tft),
                                  m_standardTime_sprite(&m_tft),
                                  m_juldate_sprite(&m_tft),
                                  m_selectionIndex(0)
{
  m_topicServer.Subscribe<DateTime>("DateTimeTopic",[this](DateTime now){
    m_datetimeNow = now;
  });

  m_topicServer.Subscribe("LeftButtonShortPressTopic", [this](){
    --m_selectionIndex;
    ChangeSelection();
  });

  m_topicServer.Subscribe("RightButtonShortPressTopic", [this](){
    ++m_selectionIndex;
    ChangeSelection();
  });

  m_topicServer.Subscribe("MiddleButtonShortPressTopic", [this](){

  });
}

void SettingsScreen::Render()
{
  TFTInitUIFrame();
}

void SettingsScreen::Loop()
{
  UpdateStandardTime(ToString(m_datetimeNow.hour()) + ":" + ToString(m_datetimeNow.minute()));
  UpdateCurrJuldate({ToString(m_datetimeNow.day()),ToString(m_datetimeNow.month()),ToString(m_datetimeNow.year())});


}

void SettingsScreen::TFTInitUIFrame()
{
  m_tft.setTextColor(m_textColor, m_bgColor);
  m_tft.fillScreen(m_bgColor);

  // DrawColHorizontalLine(40, m_tft);
  DrawColVerticalLine(TFT_WIDTH/2, m_tft);

  uint32_t vertLine = TFT_HEIGHT / m_numOfLines;
  for(int i = 1; i < m_numOfLines; ++i)
  {
    DrawColHorizontalLine((m_numOfLines-i) * vertLine, m_tft);
  }

  WriteString(StringObj("הגדרות", TFT_WIDTH - 10, 5, 5, 5, Rubik_Light28), m_fex);
  WriteString(StringObj("הגדר שעה", TFT_WIDTH - 10, 50, 5, 5, Rubik_Light20), m_fex);
  WriteString(StringObj("הגדר תאריך", TFT_WIDTH - 10, 90, 5, 5, Rubik_Light20), m_fex);
  ChangeSelection();
}

void SettingsScreen::ChangeSelection()
{
  int portion = TFT_HEIGHT / m_numOfLines;
  m_tft.fillRect(0, (portion * ((m_selectionIndex%m_numOfLines)))+1, (TFT_WIDTH/2), 39, TFT_DARKCYAN);
  m_tft.fillRect(0, (portion * ((m_selectionIndex%m_numOfLines)+1))+1, (TFT_WIDTH/2), 39, TFT_CYAN);
  m_tft.fillRect(0, (portion * ((m_selectionIndex%m_numOfLines)+2))+1, (TFT_WIDTH/2), 39, TFT_DARKCYAN);
}

void SettingsScreen::TFTInitContent()
{

}

void SettingsScreen::UpdateStandardTime(const String &standardtime_)
{
  WriteSpriteString(StringObj(standardtime_, 10, 5, 68, 18, Rubik_Light26), m_standardTime_sprite);
}

void SettingsScreen::UpdateCurrJuldate(std::vector<String> curr_juldate_)
{
  String juldate = curr_juldate_[0]+"/"+curr_juldate_[1]+"/"+curr_juldate_[2];
  WriteSpriteString(StringObj(juldate, 5, 25, 77, 13, Rubik_Light14), m_juldate_sprite);
}