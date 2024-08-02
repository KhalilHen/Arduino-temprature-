#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

// Define multiplexer control pins
const int s0 = 2;  // Control pins for CD74HC4067
const int s1 = 3;
const int s2 = 4;
const int s3 = 5;

// Define channel numbers
#define LCD_CHANNEL 2   // Channel for the LCD
#define RTC_CHANNEL 3   // Channel for the RTC

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

LiquidCrystal_I2C lcd(0x27, 16, 2);

const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage
int buzzerPin8 = 8;
unsigned long previousMillis = 0;
const long interval = 60000;
int redPin = 13;
int greenPin = 12;
int textMonitor = A1;

// Function to select a channel on the CD74HC4067 multiplexer
void selectChannel(int channel) {
  digitalWrite(s0, channel & 0x01);
  digitalWrite(s1, (channel >> 1) & 0x01);
  digitalWrite(s2, (channel >> 2) & 0x01);
  digitalWrite(s3, (channel >> 3) & 0x01);
}

void setup() {
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(buzzerPin8, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  Serial.begin(9600);

  // Initialize the I2C bus
  Wire.begin();

  // Initialize the multiplexer control pins to 0
  selectChannel(LCD_CHANNEL);
  lcd.init();
  lcd.backlight();

  // Initialize RTC
  selectChannel(RTC_CHANNEL);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}

void loop() {
  // Select the channel for the RTC and get the time
  selectChannel(RTC_CHANNEL);
  DateTime now = rtc.now();

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

  Serial.println();
  delay(3000);

  unsigned long currentMillis = millis();

  int analogValue = analogRead(A0);
  float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage

  // Simplified resistance calculation assuming no reference resistor
  float resistance = (VREF / voltage - 1) * 10000;  // Approximate the thermistor resistance

  // Calculate temperature in Kelvin using the Beta parameter
  float temperatureK = 1 / (log(resistance / R0) / BETA + 1 / T0);

  // Convert temperature from Kelvin to Celsius
  float temperatureC = temperatureK - 273.15;

  Serial.print("Temperature: ");
  Serial.println(temperatureC);

  if (temperatureC >= 26) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      noTone(buzzerPin8);
      delay(500);
    }

    // Select the channel for the LCD and update it
    selectChannel(LCD_CHANNEL);
    lcd.setCursor(0, 0);
    lcd.print("It's too hot!!");
    lcd.setCursor(0, 1);
    lcd.print("Find some coolness");
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);

    // Select the channel for the LCD and update it
    selectChannel(LCD_CHANNEL);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Normal temperature");
  }

  delay(2000); 
}
