const float BETA = 3950;  // Beta coefficient for the thermistor
const float R0 = 10000;   // Resistance of the thermistor at 25°C (10kΩ)
const float T0 = 298.15;  // Reference temperature (25°C) in Kelvin
const float VREF = 5.0;   // Reference voltage
int ledPin = 13;
void setup() {

pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(A0);
  float voltage = (analogValue / 1023.0) * VREF;  // Convert ADC value to voltage

  // Simplified resistance calculation assuming no reference resistor
  float resistance = (VREF / voltage - 1) * 10000;  // Approximate the thermistor resistance

  // Calculate temperature in Kelvin using the Beta parameter
  float temperatureK = 1 / (log(resistance / R0) / BETA + 1 / T0);

  // Convert temperature from Kelvin to Celsius
  float temperatureC = temperatureK - 273.15;

  // Output the temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temperatureC);


 if (temperatureC  >= 26)
  digitalWrite(ledPin, HIGH);

  else   {
Serial.println("De lamp moet nu uitgaan");
    digitalWrite(ledPin, LOW);
  }


  delay(2000); // Wait 2 seconds before taking another reading
}
  