#pragma once 

#include <TFT_eSPI.h>
#include <TFT_eFEX.h>
#include "TextObjects.h"

// Fonts
#include "Rubik-Light10.h"
#include "Rubik-Light12.h"
#include "Rubik-Light14.h"
#include "Rubik-Light20.h"
#include "Rubik-Light26.h"
#include "Rubik-Light28.h"

class IScreen
{
public:
    IScreen(uint32_t bgColor_ = TFT_DARKCYAN, uint32_t textColor_ = TFT_SILVER, uint32_t lineColor_ = TFT_LIGHTGREY, uint32_t numOfLines_ = 0);
    virtual void Render() = 0;

    void WriteSpriteString(const StringObj& strObj_, TFT_eSprite& tftSprite_);
    void WriteSpriteToCols(const StringArrObj& strArrObj_, TFT_eSprite& tftSprite_);

    void WriteString(const StringObj& str_, TFT_eFEX& fex_);
    void WriteToCols(const StringArrObj& str_vec_, TFT_eFEX& fex_);

    void DrawColVertLine(int x_, TFT_eSPI tft_);


protected:
    const uint32_t m_bgColor;
    const uint32_t m_textColor;
    const uint32_t m_lineColor;

    const uint32_t m_numOfLines;
};