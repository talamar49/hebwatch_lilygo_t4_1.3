#include <Arduino.h>
#include <RTClib.h>
#include "Framework/Data/TopicServer.h"



struct RTCData
{
    RTC_DS3231 m_rtc;
    TopicServer* m_topicServer;
};

void RTCInitilize(TopicServer& topicServer_);

void RTCPublishData();