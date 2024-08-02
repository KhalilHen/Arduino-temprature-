#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

// Initialize RTC and LCD
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and size if needed

const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int buzzerPin8 = 8;
unsigned long previousMillis = 0;
const long interval = 60000;
int redPin = 13;
int greenPin = 12;

void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(buzzerPin8, OUTPUT);
    pinMode(greenPin, OUTPUT);

    Serial.begin(9600);

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1); // Halt execution
    }

    // Uncomment this section only if you need to set the time
    // RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set the RTC to the date & time this sketch was compiled
}

void loop() {
    DateTime now = rtc.now();

    // Print current time to Serial Monitor
    Serial.print("Current time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    // Temperature monitoring
    int analogValue = analogRead(A0);
    float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage

    // Simplified resistance calculation
    float resistance = (VREF / voltage - 1) * R0;  // Calculate resistance

    // Calculate temperature in Kelvin and Celsius
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
            tone(buzzerPin8, 1000);
            delay(500);
            noTone(buzzerPin8);
        }

        lcd.clear();
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

    delay(2000); // Delay to update LCD less frequently
}
