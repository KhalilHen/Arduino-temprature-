#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

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


void setup() {

pinMode(redPin,  OUTPUT);
// Buzzer
  pinMode(buzzerPin8, OUTPUT);
 pinMode(greenPin, OUTPUT);
  Serial.begin(9600);

//Code for Display text LCD
  lcd.init();
  lcd.backlight();


//Code for the clock RTC
//  if (! rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     Serial.flush();
//     abort();
//   }
}



void loop() {

//Code for clock
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

  Serial.print("Temperature:");
  Serial.println(temperatureC);


 if (temperatureC  >= 26 ) {


  digitalWrite(redPin, HIGH);
 digitalWrite(greenPin, LOW);

 
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    tone(buzzerPin8, 1000); 
    delay(500);
    noTone(buzzerPin8);
  }
   lcd.setCursor(0, 0);
    lcd.print("It's too hot!!");
    lcd.setCursor(0, 1);
    lcd.print("Find some coolness");
 }

  else  {
    digitalWrite(redPin, LOW);
     digitalWrite(greenPin, HIGH);

     lcd.clear();
      lcd.setCursor(0, 0);
    lcd.print("Normal temperature");
    
      }


  delay(2000); 





}
  