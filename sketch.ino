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
  pinMode(buzzerPin8, OUTPUT);
 pinMode(greenPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
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
 
 }

  else  {
    digitalWrite(redPin, LOW);
     digitalWrite(greenPin, HIGH);

    
      }


  delay(2000); 
}
  