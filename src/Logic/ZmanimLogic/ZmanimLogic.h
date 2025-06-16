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

    ZmanimData GetZmanim(DateTime datetime);
    std::vector<ZmanimData> GetZmanimForRange(const std::vector<DateTime>& datetime);

private:    
    String GetHebDate(DateTime datetime);

    char* FormatTime(hdate date);
    hdate DateTimeToHebDate(DateTime datetime);

    location m_location;
    hdate m_habrewDate;
};