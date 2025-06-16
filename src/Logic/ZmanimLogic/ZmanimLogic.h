#pragma once
#include <RTClib.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hebrewcalendar.h>
#include <zmanim.h>
#include <hdateformat.h>
#include <shuir.h>

#include "Utils/DataNeeded.h"
#include "Framework/Data/TopicServer.h"

class ZmanimLogic
{
public:
    ZmanimLogic();

    ZmanimData GetZmanim(DateTime datetime, CityCoord coords);
    std::vector<ZmanimData> GetZmanimForRange(std::vector<DateTime> datetime, CityCoord coords);
    String GetHebDate(DateTime datetime);

private:    
    char* FormatTime(hdate date);
    hdate DateTimeToHebDate(DateTime datetime);

    location m_location;
    hdate m_habrewDate;
};