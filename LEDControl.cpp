#include "LEDControl.h"

int redPin = 22;
int greenPin = 24;
unsigned long previousMillis = 0;
const long interval = 200;

void setupLEDs() {
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
}

void handleLEDs(LiquidCrystal_I2C &lcd, float temperatureC, unsigned long currentMillis, bool turnOffBeep, int buzzerPin, int buzzerFrequency) {
    if (temperatureC >= 26) {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);

        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            if (turnOffBeep) {
                digitalWrite(redPin, HIGH);
                digitalWrite(greenPin, HIGH);
            } else {
                tone(buzzerPin, buzzerFrequency); 
                delay(500);
                noTone(buzzerPin);
            }
        }

        // Update LCD to show high temperature message
        lcd.setCursor(0, 0);
        lcd.print("It's too hot!!");
        lcd.setCursor(0, 1);
        lcd.print("Find some coolness");
    } else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);

        // Update LCD to show normal temperature message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Normal temperature");
    }
}
