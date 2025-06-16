#include "ZmanimLogic.h"

ZmanimLogic::ZmanimLogic(TopicServer &topicServer): m_topicServer(topicServer)
{
    m_topicServer.Subscribe<DateTime>("DateTimeTopic", [this](DateTime now) {
        m_dateTime = now;
    });
}

struct tm ZmanimLogic::DateTimeToStTm(DateTime& datetime)
{
    struct tm timeinfo;
    timeinfo.tm_year = m_dateTime.year() - 1900;
    timeinfo.tm_mon  = m_dateTime.month() - 1;    
    timeinfo.tm_mday = m_dateTime.day();          
    timeinfo.tm_hour = m_dateTime.hour();         
    timeinfo.tm_min  = m_dateTime.minute();       
    timeinfo.tm_sec  = m_dateTime.second();       
    timeinfo.tm_isdst = 0;

    return timeinfo;
}

char* ZmanimLogic::FormatTime(hdate date)
{
	static char final[32];
	final[0] = '\0';
	time_t time = hdatetime_t(date);
	struct tm *tm = localtime(&time);
	strftime(final, 31, "%I:%M %p %Z", tm);
	return final;
}

void ZmanimLogic::MainLogic()
{
    hdate hebrewDate = convertDate(DateTimeToStTm(m_dateTime));
    hebrewDate.offset = 0;
    setEY(&hebrewDate, 1);

    char today[32] = {"\0"};
	hdateformat(today, 32, hebrewDate);

	std::cout << "tdaslfkj" << std::endl;
}