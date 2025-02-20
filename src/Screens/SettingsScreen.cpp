#include "Screens/SettingsScreen.h"

SettingsScreen::SettingsScreen(TFT_eSPI tft_, TopicServer& topicServer_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                                : IScreen(topicServer_, bgColor_, textColor_, lineColor_, numOfLines_),
                                  m_tft(tft_),
                                  m_fex(&m_tft)
{}

void SettingsScreen::Render()
{
    m_tft.setTextColor(m_textColor, m_bgColor);
    m_tft.fillScreen(m_bgColor);
}