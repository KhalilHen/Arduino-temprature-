#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Wire.h>
#include <SevSeg.h>


// Pins fRTC_DS1307 rtc;

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SevSeg sevseg;


const int COLON_PIN = 13;

const  uint8_t  digitPins[] = {2, 3, 4, 5}; // Pins connected to each digit's common pin
const  uint8_t  segmentPins[] = {6, 7, 8, 9, 10, 11, 12}; // Pins connected to segments a-g

const byte digitPatterns[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// Initialize RTC and LCD

const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage

int buzzerPin = 8;
unsigned long previousMillis = 0;
const long interval = 60000;
int redPin = 11;
int greenPin = 12;

void setup() {
    Serial.begin(9600);

    // Set pin modes for 7-segment display
    for (int i = 0; i < 7; i++) {
        pinMode(segmentPins[i], OUTPUT);
    }
    for (int i = 0; i < 4; i++) {
        pinMode(digitPins[i], OUTPUT);
    }

    pinMode(redPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(COLON_PIN, OUTPUT);

    // Initialize LCD
    lcd.init();
    lcd.backlight();

    // Initialize RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt execution
    }
 pinMode(COLON_PIN, OUTPUT);

 byte digits = 4;
 bool resistorsOnSegments = false;
    bool updateWithDelays = false;
    bool leadingZeros = true;
    bool disableDecPoint = true;
    sevseg.begin(COMMON_ANODE, digits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);

    sevseg.setBrightness(90);

}


   


void loop() {

    DateTime now = rtc.now();

    // Display hours and minutes on 7-segment display
    int hours = now.hour();
    int minutes = now.minute();
  sevseg.setNumber(hours * 100 + minutes);
    sevseg.refreshDisplay();
    bool blinkState = now.second() % 2 == 0;

    setColon(blinkState);



  // Temperature monitoring
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
        digitalWrite(redPin, 255);
        digitalWrite(greenPin, 0);

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
        lcd.print(temperatureC, 1); // Display temperature with 1 decimal place
        lcd.print(" C");
    } else {
        digitalWrite(redPin, 0);
        digitalWrite(greenPin, 255);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature:");
        lcd.setCursor(0, 1);
        lcd.print(temperatureC, 1); // Display temperature with 1 decimal place
        lcd.print(" C");
    }

    delay(2000); // Delay to update LCD less frequently
}

// void displayDigit(int digit, int position) {
//     // Turn off all digits
//     for (int i = 0; i < 4; i++) {
//         digitalWrite(digitPins[i], LOW);
//     }

//     // Set the digit pattern
//     byte pattern = digitPatterns[digit];
//     for (int i = 0; i < 7; i++) {
//         digitalWrite(segmentPins[i], (pattern & (1 << i)) ? HIGH : LOW);
//     }

//     // Turn on the selected digit
//     digitalWrite(digitPins[position], HIGH);

//     // Debug output
//     Serial.print("Displaying digit: ");
//     Serial.print(digit);
//     Serial.print(" on position: ");
//     Serial.println(position);
// }


void setColon(bool value) {
  digitalWrite(COLON_PIN, value ? LOW : HIGH);
}



// void displayTime() {
//   DateTime now = rtc.now();
//   char timeString[17]
//     sprint(timeString, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

//   bool blinkState = now.second() % 2 == 0;
//   sevseg.setNumber(now.hour() * 100 + now.minute());
//   setColon(blinkState);
// }
