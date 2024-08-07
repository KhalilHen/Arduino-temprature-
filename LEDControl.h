#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Functions
void setupLEDs();
void handleLEDs(LiquidCrystal_I2C &lcd, float temperatureC, unsigned long currentMillis, bool turnOffBeep, int buzzerPin, int buzzerFrequency);

#endif
