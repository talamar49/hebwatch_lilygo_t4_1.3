#include "RTCHelper.h"

RTCData rtcData;

void RTCInitilize(TopicServer& topicServer_)
{
    rtcData.m_topicServer = &topicServer_;
    if (!rtcData.m_rtc.begin()) 
    {
        Serial.println("Couldn't find RTC");
    }

    // Check if the RTC lost power and if so, set the time
    // if (rtcData.m_rtc.lostPower()) 
    // {
    //     Serial.println("RTC lost power, setting the time!");
        // Set the time to a known value (e.g., Jan 1, 2023)
    //     rtcData.m_rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
    // }
    
    rtcData.m_topicServer->Subscribe<DateTime>("AdjustDateTimeTopic",[](DateTime adjustTime){
        rtcData.m_rtc.adjust(adjustTime);
    });
}

void RTCPublishData()
{
    rtcData.m_topicServer->Publish<DateTime>("DateTimeTopic",rtcData.m_rtc.now());
}