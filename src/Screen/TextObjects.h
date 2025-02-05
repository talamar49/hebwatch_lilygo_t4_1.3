#include <Arduino.h>
#include <vector>

class TextObj
{
public:
    TextObj(const int32_t x_, const  int32_t y_, const int32_t width_, const int32_t height_, const uint8_t *font_) 
            : x(x_),
              y(y_),
              width(width_),
              height(height_),
              font(font_)
              {}

    const int32_t x; 
    const int32_t y;
    const int32_t width; 
    const int32_t height;
    const uint8_t* font;
};

class StringObj : public TextObj
{
public:
    StringObj(const String str_, const int32_t x_, const  int32_t y_, const int32_t width_, const int32_t height_, const uint8_t *font_)
            : TextObj(x_, y_, width_, height_, font_),
              str(str_)
              {}

    String str;
};

class StringArrObj : public TextObj
{
public:
    StringArrObj(const std::vector<const String> str_arr_, const int32_t x_, const  int32_t y_, const int32_t width_, const int32_t height_, const uint8_t *font_)
                : TextObj(x_, y_, width_, height_, font_),
                  str_arr(str_arr_)
                  {}
    
    const std::vector<const String> str_arr;
};