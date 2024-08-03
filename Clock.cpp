#include <Arduino.h>
#include "Clock.h"

#define MINUTE 60 * 1000L /* ms */
#define TIMESPAN_DAY TimeSpan(1, 0, 0, 0)



Clock::Clock()
  {
}

void Clock::begin() {
# if USE_RTC
  if (!_rtc.begin()) {
    Serial.println("Couldn't find RTC");
    abort();
  }
# else /* USE_RTC */
  DateTime zeroTime;
  _rtc.begin(zeroTime);
# endif
}

/***** Clock management *****/

DateTime Clock::now() {
  return _rtc.now();
}

void Clock::incrementMinute() {
  DateTime now = _rtc.now();
  DateTime newTime = DateTime(now.year(), now.month(), now.day(), now.hour(),
                              (now.minute() + 1) % 60);
  _rtc.adjust(newTime);
}

void Clock::incrementHour() {
  DateTime now = _rtc.now();
  DateTime newTime = DateTime(now.year(), now.month(), now.day(),
                              (now.hour() + 1) % 24, now.minute());
  _rtc.adjust(newTime);
}

/***** Alarm management *****/



