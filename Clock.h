#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <RTClib.h>
#include "config.h"



class Clock {
public:
  Clock();
  void begin();

  /* Clock management */
  DateTime now();
  void incrementMinute();
  void incrementHour();

  /* Alarm Management */
  void incrementAlarmHour();
  void incrementAlarmMinute();

protected:
  bool _isAlarmDueTime();

  #if USE_RTC
    RTC_DS1307 _rtc;
  #else /* USE_RTC */
    RTC_Millis _rtc;
  #endif /* USE_RTC */

  };

#endif /* __CLOCK_H__ */
