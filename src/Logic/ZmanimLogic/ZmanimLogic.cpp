#include "ZmanimLogic.h"

ZmanimLogic::ZmanimLogic()
{
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


ZmanimData ZmanimLogic::GetZmanim(DateTime datetime, CityCoord coords)
{
    ZmanimData zmanimData;

    hdate hebrewDate = DateTimeToHebDate(datetime);
    setEY(&hebrewDate, 1);

    char today[32] = {"\0"};
    hdateformat(today, 32, hebrewDate);

    
    // Set location (latitude, longitude, elevation)
    location loc = {coords.latitude, coords.longitude, coords.elevation};
    
    zmanimData.hebrewDate = String(today);
    // Calculate Zmanim using libzmanim
    zmanimData.alos = FormatTime(getalos(hebrewDate, loc));
    zmanimData.misheyakir = FormatTime(getmisheyakir11degrees(hebrewDate, loc));
    zmanimData.sunrise = FormatTime(getsunrise(hebrewDate, loc));
    zmanimData.shma = FormatTime(getshmagra(hebrewDate, loc));
    zmanimData.tefila = FormatTime(gettefilagra(hebrewDate, loc));
    zmanimData.chatzos = FormatTime(getchatzosgra(hebrewDate, loc));
    zmanimData.minchaGedola = FormatTime(getminchagedolagra(hebrewDate, loc));
    zmanimData.minchaKetana = FormatTime(getminchaketanagra(hebrewDate, loc));
    zmanimData.plagHamincha = FormatTime(getplaggra(hebrewDate, loc));
    zmanimData.sunset = FormatTime(getsunset(hebrewDate, loc));
    zmanimData.tzais = FormatTime(gettzais8p5(hebrewDate, loc));

    // Additional calculations (e.g., Shabbat times, molad, etc.)
    zmanimData.shabatStart = FormatTime(getcandlelighting(hebrewDate, loc));
    zmanimData.shabatEnd = FormatTime(gettzais8p5(hebrewDate, loc));
    zmanimData.shabatTam = FormatTime(gettzaisbaalhatanya(hebrewDate, loc));
    // zmanimData.molad = FormatTime(getmolad(hebrewDate));

    // Populate other fields (e.g., parasha, yomtov, etc.)
    zmanimData.parasha = getparshah(hebrewDate);
    zmanimData.yomtov = getyomtov(hebrewDate);
    zmanimData.roshChodesh = getroshchodesh(hebrewDate);
    zmanimData.shabbosMevorchim = getshabbosmevorchim(hebrewDate);
    zmanimData.macharChodesh = getmacharchodesh(hebrewDate);
    zmanimData.specialShabbos = getspecialshabbos(hebrewDate);
    zmanimData.omerCount = getomer(hebrewDate);
    // zmanimData.avosChapter = getavoschapter(hebrewDate);
    zmanimData.birchasHashanim = getbirchashashanim(hebrewDate);

    // Populate daily shiurim
    // zmanimData.chumashShiur = getchumashshiur(hebrewDate);
    // zmanimData.tehillimShiur = gettehillimshiur(hebrewDate);
    // zmanimData.tanyaShiur = gettanyashiur(hebrewDate);
    // zmanimData.rambamShiur = getrambamshiur(hebrewDate);

    return zmanimData;
}

std::vector<ZmanimData> ZmanimLogic::GetZmanimForRange(std::vector<DateTime> datetime, CityCoord coords)
{
    std::vector<ZmanimData> zmanimVec; 
    zmanimVec.resize(7); 

    for (int i = 0; i < 7; ++i) {
        zmanimVec[i] = GetZmanim(datetime[i],coords);
    }
    
    return zmanimVec;
}