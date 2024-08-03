#include "DisplayControl.h"
#include ''a
Clock clock;
SevSeg sevseg;

DisplayState displayState = DisplayClock;
long lastStateChange = 0;

void changeDisplayState(DisplayState newValue) {
  displayState = newValue;
  lastStateChange = millis();
}

long millisSinceStateChange() {
  return millis() - lastStateChange;
}

void setColon(bool value) {
  digitalWrite(COLON_PIN, value ? LOW : HIGH);
}

void displayTime() {
  DateTime now = clock.now();
  bool blinkState = now.second() % 2 == 0;
  sevseg.setNumber(now.hour() * 100 + now.minute());
  setColon(blinkState);
}

void clockState() {
  displayTime();
}



void setupDisplay() {
  pinMode(COLON_PIN, OUTPUT);

  byte digits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12};
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = true;
  sevseg.begin(DISPLAY_TYPE, digits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}


void displayTemperature() {
  // Assuming you have a function or variable that provides the temperature
  float temperature = getTemperature(); // Replace with actual temperature retrieval
  sevseg.setNumber(temperature * 10); // Display temperature with 1 decimal place
  setColon(false); // No need for colon in temperature display
}