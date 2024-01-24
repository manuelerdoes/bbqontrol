#include <Arduino.h>
#include "max6675.h"

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

// Setting PWM properties
const int freq = 180;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;


// thermostuff max6675
int thermoDO = 5;
int thermoCS = 18;
int thermoCLK = 19;

float targetTemp = 30.0;
float currentTemp = 30.0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.begin(9600);

  // testing
  Serial.print("Testing DC Motor...");
  Serial.println("MAX6675 test");

  // wait for MAX chip to stabilize
  delay(500);

    
}

void loop() {


 // basic readout test, just print the current temp
  
  Serial.print("C = "); 
  Serial.println(thermocouple.readCelsius());
  Serial.print("F = ");
  Serial.println(thermocouple.readFahrenheit());
  currentTemp = thermocouple.readCelsius();
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(1000);

  // Move DC motor forward 
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  if (currentTemp >= targetTemp) {
    dutyCycle = 0;
  } else if (targetTemp - currentTemp < 5) {
    dutyCycle = 60;
  } else if (targetTemp - currentTemp < 10) {
    dutyCycle = 120;
  }

  ledcWrite(pwmChannel, dutyCycle);
}