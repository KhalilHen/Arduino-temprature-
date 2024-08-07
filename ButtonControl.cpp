#include "ButtonControl.h"


#define buttonPin 28

int oldButtonValue = LOW; 
// bool turnOffBeep = false;
extern bool turnOffBeep;

void setupButton() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool checkButtonState() {
    int buttonValue = digitalRead(buttonPin);

    if (buttonValue != oldButtonValue) {
        Serial.println(buttonValue);
        delay(50);
        buttonValue = digitalRead(buttonPin);
        if (buttonValue == LOW) {
            turnOffBeep = !turnOffBeep;
        }
        oldButtonValue = buttonValue;
    }
    return turnOffBeep;
}
