#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include <SevSeg.h>
#include "Clock.h"
#include "config.h"

const int COLON_PIN = 13;
const int SPEAKER_PIN = A3;

extern Clock clock;
extern SevSeg sevseg;

enum DisplayState {
  DisplayClock,
  DisplaySnooze,
};

extern DisplayState displayState;
extern long lastStateChange;

void changeDisplayState(DisplayState newValue);
long millisSinceStateChange();
void setColon(bool value);
void displayTime();
void clockState();
void alarmState();
void setupDisplay();

#endif // DISPLAYCONTROL_H
