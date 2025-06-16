#pragma once
#include <Arduino.h> // For String type
#include <vector>

struct CityCoord {
    float latitude;
    float longitude;
    float elevation; // Added elevation to the CityCoord struct
};


// Define ZmanimData structure to hold all calculated zmanim and Hebrew calendar info
struct ZmanimData {
    // Hebrew Date
    String hebrewDate;

    // Parasha and special days
    String parasha;
    String yomtov;
    String roshChodesh;
    String shabbosMevorchim;
    String macharChodesh;
    String specialShabbos;
    String omerCount;
    String avosChapter;
    String birchasHashanim; // "ותן טל ומטר" or "ותן ברכה"

    // Zmanim (times)
    String alos;
    String misheyakir;
    String sunrise;
    String shma;
    String tefila;
    String chatzos;
    String minchaGedola;
    String minchaKetana;
    String plagHamincha;
    String candleLighting; // Could be multiple types (regular, nightfall, chanukah)
    String sunset;
    String tzais;          // Could be multiple types (8.5, 72, baalhatanya)

    // Shabbat times (if applicable, for the current week)
    String shabatStart; // Candle lighting for Shabbos/Yom Tov (if applicable)
    String shabatEnd;   // Tzeis for Shabbos/Yom Tov (if applicable)
    String shabatTam;   // Tzeis Tumas Ha'aretz (e.g., 45 minutes after sunset)

    // Molad
    String molad; // e.g., "HH:MM:SS on Wday"

    // Daily Shiurim
    String chumashShiur;
    String tehillimShiur;
    String tanyaShiur;
    String rambamShiur;
};