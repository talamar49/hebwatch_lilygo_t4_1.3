#include "Framework/Screen/IScreen.h"

IScreen::IScreen(TopicServer& topicServer_, uint32_t bgColor_, uint32_t textColor_, uint32_t lineColor_, uint32_t numOfLines_)
                :
                m_topicServer(topicServer_), 
                m_bgColor(bgColor_),
                m_textColor(textColor_),
                m_lineColor(lineColor_),
                m_numOfLines(numOfLines_)
{}


void IScreen::WriteToCols(const StringArrObj& strArrObj_, TFT_eFEX& fex_)
{
  for(int i = 1; i < m_numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / m_numOfLines) * (i);

    StringObj strObj = StringObj(strArrObj_.str_arr[i-1], strArrObj_.x, strArrObj_.y + portion, strArrObj_.width, strArrObj_.height, strArrObj_.font);

    WriteString(strObj, fex_);
  }
}


void IScreen::WriteString(const StringObj& strObj_, TFT_eFEX& fex_, const uint32_t bgColor_)
{
  fex_.GetESpi()->loadFont(strObj_.font);
  
  fex_.setTextColor(m_textColor, m_bgColor);

  if(bgColor_ == 0)
  {
    fex_.fillRect(strObj_.x - strObj_.width, strObj_.y, strObj_.width, strObj_.height, m_bgColor); 
  }
  else
  {
    fex_.fillRect(strObj_.x - strObj_.width, strObj_.y, strObj_.width, strObj_.height, bgColor_); 
  }

  fex_.setCursorRTL(strObj_.x, strObj_.y);
  fex_.drawStringRTL(strObj_.str);

  fex_.GetESpi()->unloadFont();
}

void IScreen::WriteSpriteToCols(const StringArrObj& strArrObj_, TFT_eSprite& tftSprite_)
{
  for(int i = 1; i < m_numOfLines; ++i)
  {
    int portion = (TFT_HEIGHT / m_numOfLines) * (i);

    StringObj strObj = StringObj(strArrObj_.str_arr[i-1], strArrObj_.x, strArrObj_.y + portion, strArrObj_.width, strArrObj_.height, strArrObj_.font);
    
    WriteSpriteString(strObj, tftSprite_);
  }
}

void IScreen::WriteSpriteString(const StringObj& strObj_, TFT_eSprite& tftSprite_, const uint32_t bgColor_)
{
  tftSprite_.loadFont(strObj_.font);

  tftSprite_.createSprite(strObj_.width, strObj_.height);
  tftSprite_.setTextColor(m_textColor, m_bgColor);
  if(bgColor_ == 0)
  {
    tftSprite_.fillScreen(m_bgColor);
  }
  else
  {
    tftSprite_.fillScreen(bgColor_);
  }
  
  tftSprite_.drawString(strObj_.str, 0, 0);
  tftSprite_.pushSprite(strObj_.x, strObj_.y);

  tftSprite_.unloadFont();
}

void IScreen::WriteTFTString(const StringObj& strObj_, TFT_eSPI& tft_)
{
  tft_.loadFont(strObj_.font);

  tft_.drawString(strObj_.str, strObj_.x, strObj_.y);

  tft_.unloadFont();
}

void IScreen::DrawColVerticalLine(int x_, TFT_eSPI tft_)
{
  tft_.drawLine(x_, 40, x_, TFT_HEIGHT ,m_lineColor);
}

void IScreen::DrawColHorizontalLine(int y_, TFT_eSPI tft_)
{
  tft_.drawLine(0, y_, TFT_WIDTH, y_ ,m_lineColor);
}

String IScreen::ToString(int number) {
    // Check if the number is between 0 and 9
    if (number >= 0 && number <= 9) {
        // Add a leading zero and return as a String
        return "0" + String(number);
    } else {
        // Return the number as a String without modification
        return String(number);
    }
}
