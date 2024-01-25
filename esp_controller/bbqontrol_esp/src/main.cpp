#include <Arduino.h>
#include "max6675.h"
#include <LiquidCrystal_I2C.h>
#include <ESP_Knob.h>

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

float targetTemp = 22.0;
float currentTemp = 30.0;
float minTarget = 45.0;
float maxTarget = 300.0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// LCD stuff
#define I2C_SDA 4
#define I2C_SCL 16

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


// rotary encoder 
#define GPIO_NUM_KNOB_PIN_A     34
#define GPIO_NUM_KNOB_PIN_B     35

ESP_Knob *knob;

void onKnobLeftEventCallback(int count, void *usr_data)
{
    if (targetTemp > minTarget) 
    {
      targetTemp -= 1.0;
    }
}

void onKnobRightEventCallback(int count, void *usr_data)
{
    if (targetTemp < maxTarget)
    {
      targetTemp += 1.0;
    }
}

int counter = 0;
float difference = 0.0;


void setup() {

	lcd.init(I2C_SDA, I2C_SCL); // initialize the lcd to use user defined I2C pins
	lcd.backlight();
	lcd.setCursor(3,0);
  lcd.print("BBQontrol");

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

  // knob setup
  knob = new ESP_Knob(GPIO_NUM_KNOB_PIN_A, GPIO_NUM_KNOB_PIN_B);
  knob->begin();
  knob->attachLeftEventCallback(onKnobLeftEventCallback);
  knob->attachRightEventCallback(onKnobRightEventCallback);

    
}

void loop() {

if (counter % 5 == 0) {

  Serial.print("C = "); 
  Serial.println(thermocouple.readCelsius());
  Serial.print("F = ");
  Serial.println(thermocouple.readFahrenheit());
  currentTemp = thermocouple.readCelsius();
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(250);
}

  lcd.setCursor(0,0);
  lcd.print("Current: " + String(currentTemp) + "C");
  lcd.setCursor(0,1);
  lcd.print("Target: " + String(targetTemp) + "C");

  

  // Move DC motor forward 
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);


  if (currentTemp >= targetTemp) {
    dutyCycle = 0;
  // } else if (targetTemp - currentTemp < 5) {
  //   dutyCycle = 60;
  // } else if (targetTemp - currentTemp < 10) {
  //   dutyCycle = 120;
  } else {
    difference = targetTemp - currentTemp;
    if (difference > 100) {
      difference = 100;
    }
    dutyCycle = ((difference - 1) / 99) * 225 + 30;
  }

  ledcWrite(pwmChannel, dutyCycle);
  counter++;
  delay(100);
}