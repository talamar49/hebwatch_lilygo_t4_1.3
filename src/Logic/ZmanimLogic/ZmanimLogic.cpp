#include "ZmanimLogic.h"

ZmanimLogic::ZmanimLogic()
{
    std::cout <<"build" <<std::endl;
}

hdate ZmanimLogic::DateTimeToHebDate(DateTime datetime)
{
    struct tm timeinfo;
    timeinfo.tm_year = datetime.year() - 1900;
    timeinfo.tm_mon  = datetime.month() - 1;    
    timeinfo.tm_mday = datetime.day();          
    timeinfo.tm_hour = datetime.hour();         
    timeinfo.tm_min  = datetime.minute();       
    timeinfo.tm_sec  = datetime.second();       
    timeinfo.tm_isdst = 0;

    return convertDate(timeinfo);
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

String ZmanimLogic::GetHebDate(DateTime datetime)
{
    hdate habrewDate = DateTimeToHebDate(datetime);
    setEY(&habrewDate, 1);

    char today[32] = {"\0"};
    hdateformat(today, 32, habrewDate);

    return String(today);
}


ZmanimData ZmanimLogic::GetZmanim(DateTime datetime)
{
    ZmanimData zmanimData;

    zmanimData.hebrewDate = GetHebDate(datetime);

    return zmanimData;
}

std::vector<ZmanimData> ZmanimLogic::GetZmanimForRange(const std::vector<DateTime>& datetime)
{
    std::vector<ZmanimData> zmanimVec;

    // for (int i = 0; i < 7; ++i) {
    //     zmanimVec[i].hebrewDate = GetHebDate(datetime[i]);
    // }
    
    return zmanimVec;
}