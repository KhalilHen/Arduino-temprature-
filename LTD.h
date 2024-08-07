#ifndef LTD_H
#define LTD_H

#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SevSeg.h>

void setupLTD();
void loopLTD();

// Global variables
extern RTC_DS1307 rtc;
extern LiquidCrystal_I2C lcd;
extern SevSeg sevseg;
extern bool turnOffBeep;
extern int oldButtonValue;
extern unsigned long previousMillis;
extern const long interval;

#endif // LTD_H
