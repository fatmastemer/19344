#include "DHT.h"
#include <SoftwareSerial.h>

#define DHTPIN 2       // Pin connected to DHT sensor
#define DHTTYPE DHT11  // DHT 11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

#define buzzer 5
#define red 9
#define led 8
#define measurePin A0 // Connect dust sensor to Arduino A0 pin
#define ledPower 7    // Connect LED driver pin of dust sensor to Arduino D2

int samplingTime = 280; // Time required to sample signal from the sensor
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

SoftwareSerial bt(3, 4); // Pins for SoftwareSerial
String x;

// Declare the variable `xx` here
int xx; // Variable to store temperature

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  
  pinMode(ledPower, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(led, OUTPUT);
  
  Serial.println("Humidity & Temperature & Dust levels");
  dht.begin();
}

void loop() {
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  

  float hi = dht.computeHeatIndex(f, h);


  

  // Dust sensor reading
  digitalWrite(ledPower, LOW); // Power on the LED
  voMeasured = analogRead(measurePin); // Read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // Turn the LED off
  delayMicroseconds(sleepTime);
  
  calcVoltage = voMeasured * (5.0 / 1024.0);
  
  // Update the dust density calculation with your new formula
  dustDensity = (0.170 * calcVoltage - 0.1) * 100; // Example formula for dust density in µg/m³
  if(dustDensity<0){
    dustDensity*=-1;
  }
  //Serial.print("Dust Density: ");
  Serial.print(h);
  Serial.print(",");
  //Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(",");
  Serial.println(dustDensity);

  // Update the variable `xx` with the temperature
  xx = t; // Example: send the temperature to Bluetooth

  // Control buzzer based on temperature
  if (t > 30) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }
  
  // Control red LED based on humidity
  if (h > 70) {
    digitalWrite(red, HIGH);
  } else {
    digitalWrite(red, LOW);
  }
  
  // Control dust sensor LED
  if (dustDensity > 5) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  delay(500); // Keep the LED state for 500 milliseconds

  // Send temperature data over Bluetooth
  bt.print(xx); 

  // Check for incoming Bluetooth commands
  if (bt.available()) {
    x = bt.readString();
    if (x == "xx") {
      // Add your action for "xx" command here
    } else if (x == "anotherCommand") {
      // Add your action for another command here
    }
  }
  
  x = ""; // Clear the string for the next iteration
  delay(1000); // Delay before the next loop iteration
}
