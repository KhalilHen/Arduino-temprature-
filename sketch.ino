#include <SevSeg.h>
#include "Clock.h"
#include "config.h"
#include "DisplayControl.h"
#include <LiquidCrystal_I2C.h>
#include <RTClib.h> 
#include <Wire.h>
#include "LEDControl.h"
#include "ButtonControl.h"



// RTC and LCD initialization
RTC_DS1307 rtc;  
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button code
#define buttonPin 28

int sliderPin = A1;  
int sliderValue = 0;
bool turnOffBeep = false;

// Temperature constants
const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage
// Pin definitions
int buzzerPin = 41;

void setup() {
      setupButton();

    setupLEDs();
    pinMode(buzzerPin, OUTPUT);
    pinMode(COLON_PIN, OUTPUT);
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
    turnOffBeep = checkButtonState(); // Update the beep state based on button press

    // Read the slider value
    sliderValue = analogRead(sliderPin);
 
    int buzzerFrequency = map(sliderValue, 0, 1023, 100, 2000); // Map slider value to 100-2000 Hz

    unsigned long currentMillis = millis();
    int analogValue = analogRead(A0);
    float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage

    // Simplified resistance calculation assuming no reference resistor
    float resistance = (VREF / voltage - 1) * 10000;  // Approximate the thermistor resistance

    // Calculate temperature in Kelvin using the Beta parameter
    float temperatureK = 1 / (log(resistance / R0) / BETA + 1 / T0);

    // Convert temperature from Kelvin to Celsius
    float temperatureC = temperatureK - 273.15;

    handleLEDs(lcd, temperatureC, currentMillis, turnOffBeep, buzzerPin, buzzerFrequency);

    delay(2000); 

}
