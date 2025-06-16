#pragma once
#include <RTClib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <hebrewcalendar.h>
#include <zmanim.h>
#include <hdateformat.h>
#include <shuir.h>

#include "Framework/Data/TopicServer.h"

class ZmanimLogic
{
public:
    ZmanimLogic(TopicServer &topicServer);
    void MainLogic();

private:

    char* FormatTime(hdate date);
    struct tm DateTimeToStTm(DateTime& datetime);

    TopicServer &m_topicServer;
    DateTime m_dateTime;
    location m_location;
};