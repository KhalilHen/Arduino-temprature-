#include <SevSeg.h>
#include "Clock.h"
#include "config.h"
#include "DisplayControl.h"
#include <LiquidCrystal_I2C.h>
#include <RTClib.h> 
#include <Wire.h>

// RTC and LCD initialization
RTC_DS1307 rtc;  
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button code
#define buttonPin 28

int sliderPin = A1;  
int sliderValue = 0;
int oldButtonValue = LOW; 
bool turnOffBeep = false;

// Temperature constants
const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage
// Pin definitions
int buzzerPin = 41;
unsigned long previousMillis = 0;
const long interval = 200;
int redPin = 22;
int greenPin = 24;

void setup() {
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(COLON_PIN, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(sliderPin, INPUT); // Initialize slider pin

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt execution
    }

    clock.begin();
    setupDisplay();
}

void loop() {
    int buttonValue = digitalRead(buttonPin);

    if (buttonValue != oldButtonValue) {
        Serial.println(buttonValue);
        delay(50);
        buttonValue = digitalRead(buttonPin);
        if (buttonValue == LOW) { 
            turnOffBeep = !turnOffBeep; 
            Serial.println(turnOffBeep ? "Beep is turned off" : "Beep is turned on");
            Serial.println("The button is pressed.");
        }
        oldButtonValue = buttonValue;
    }

    // Read the slider value
    sliderValue = analogRead(sliderPin);
 
    int buzzerFrequency = map(sliderValue, 0, 1023, 100, 2000); // Map slider value to 100-2000 Hz

    // Temporary not using
    // sevseg.refreshDisplay();

    // switch (displayState) {
    //     case DisplayClock:
    //         clockState();
    //         delay(5000);
    //         changeDisplayState(DisplayTemperature);
    //         break;
    // }
    
    unsigned long currentMillis = millis();
    int analogValue = analogRead(A0);
    float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage

    // Simplified resistance calculation assuming no reference resistor
    float resistance = (VREF / voltage - 1) * 10000;  // Approximate the thermistor resistance

    // Calculate temperature in Kelvin using the Beta parameter
    float temperatureK = 1 / (log(resistance / R0) / BETA + 1 / T0);

    // Convert temperature from Kelvin to Celsius
    float temperatureC = temperatureK - 273.15;

    if (temperatureC >= 26) {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);

        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            if (turnOffBeep) {
                Serial.println("Beep turned off");
                digitalWrite(redPin, HIGH);
                digitalWrite(greenPin, HIGH);
            } else {
                tone(buzzerPin, buzzerFrequency); 
                delay(500);
                noTone(buzzerPin);
            }
        }

        lcd.setCursor(0, 0);
        lcd.print("It's too hot!!");
        lcd.setCursor(0, 1);
        lcd.print("Find some coolness");
    } else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Normal temperature");
    }

    delay(2000); 
}
