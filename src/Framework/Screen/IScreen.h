#pragma once 

#include <TFT_eSPI.h>
#include <TFT_eFEX.h>
#include "TextObjects.h"
#include "Framework/Data/TopicServer.h"

// Fonts
#include "Fonts/Rubik-Light10.h"
#include "Fonts/Rubik-Light12.h"
#include "Fonts/Rubik-Light14.h"
#include "Fonts/Rubik-Light20.h"
#include "Fonts/Rubik-Light26.h"
#include "Fonts/Rubik-Light28.h"

class IScreen
{
public:
    IScreen(TopicServer& topicServer_, uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 0);
    virtual void Render() = 0;
    virtual void Loop() = 0;

    void WriteSpriteString(const StringObj& strObj_, TFT_eSprite& tftSprite_, const uint32_t bgColor_ = 0);
    void WriteSpriteToCols(const StringArrObj& strArrObj_, TFT_eSprite& tftSprite_);

    void WriteString(const StringObj& str_, TFT_eFEX& fex_);
    void WriteToCols(const StringArrObj& str_vec_, TFT_eFEX& fex_);

    void WriteTFTString(const StringObj& strObj_, TFT_eSPI& tft_);

    void DrawColVerticalLine(int x_, TFT_eSPI tft_);
    void DrawColHorizontalLine(int y_, TFT_eSPI tft_);

    String ToString(int number_);

protected:
    TopicServer& m_topicServer;
    const uint32_t m_bgColor;

    const uint32_t m_textColor;
    const uint32_t m_lineColor;

    const uint32_t m_numOfLines;
};