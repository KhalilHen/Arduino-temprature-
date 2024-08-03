//Clock includes
#include <SevSeg.h>

#include "Clock.h"
#include "config.h"
#include "DisplayControl.h"

#include <LiquidCrystal_I2C.h>
#include <RTClib.h> 
// #include <Wire.h>



//  RTC and LCD initialization
 RTC_DS1307 rtc;  
 LiquidCrystal_I2C lcd(0x27, 16, 2);


enum DisplayState { DisplayClock, DisplayTemperature };
DisplayState displayState = DisplayClock;


// Temperature constants
const float BETA = 3950;  // Beta coefficient for the thermistor

const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage

// Pin definitions
int buzzerPin = 8;
unsigned long previousMillis = 0;
const long interval = 60000;
int redPin = 11;
int greenPin = 12;






void setup() {





   pinMode(redPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(COLON_PIN, OUTPUT);

Serial.begin(9600);
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



  sevseg.refreshDisplay();

  switch (displayState) {
    case DisplayClock:
      clockState();

      delay(2000);
       displayState = DisplayTemperatureText; 
      break;
      case DisplayTemperatureText:

      
  int analogValue = analogRead(A0);
    float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage
   float resistance = (VREF / voltage - 1) * R0;  // Calculate resistance
     float temperatureK = 1 / (log(resistance / R0) / BETA + 1 / T0);
  float temperatureC = temperatureK - 273.15;

     Serial.print("Temperature: ");
    Serial.println(temperatureC);

    // Temperature-based control
    unsigned long currentMillis = millis();
    if (temperatureC >= 26) {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);

        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            tone(buzzerPin, 1000);
            delay(500);
            noTone(buzzerPin);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Too hot! Temp:");
                lcd.setCursor(0, 1);
        lcd.print("Find some coolness");

     
    } else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
   lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Normal temperature");
    }


     delay(2000); // Delay to update LCD less frequently



   DisplayTemperatureText = DisplayClock;

      }
      default: break; 
  }
   
  
}