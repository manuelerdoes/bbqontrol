#include <Arduino.h>
#include "max6675.h"
#include <LiquidCrystal_I2C.h>
#include <ESP_Knob.h>
#include <WiFi.h>

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

float targetTemp = 45.0;
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
      targetTemp -= 0.5;
    }
    // delay(50);
}

void onKnobRightEventCallback(int count, void *usr_data)
{
    if (targetTemp < maxTarget)
    {
      targetTemp += 0.5;
    }
    // delay(50);
}

// WiFi
// const char* ssid = "BabaWaves69";
// const char* password = "NurFuerBaba5";
const char* ssid = "manuel";
const char* password = "asdf1234";

// utils

int loopCounter = 0;
int fanRunningCounter = 0;
float difference = 0.0;
float lastTemp = 0.0;
int isSinkingCounter = 0;
float olderTemp = 0.0;
float tempDiff = 0.0;


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

  // wait for MAX chip to stabilize
  delay(500);

  // knob setup
  knob = new ESP_Knob(GPIO_NUM_KNOB_PIN_A, GPIO_NUM_KNOB_PIN_B);
  knob->begin();
  knob->attachLeftEventCallback(onKnobLeftEventCallback);
  knob->attachRightEventCallback(onKnobRightEventCallback);

  // wifi setup
    // WiFi.mode(WIFI_STA); //Optional
    // WiFi.begin(ssid, password);
    // Serial.println("\nConnecting");

    // while(WiFi.status() != WL_CONNECTED){
    //     Serial.println("status: " + String(WiFi.status()));
    //     delay(1000);
    // }

    // Serial.println("\nConnected to the WiFi network");
    // Serial.print("Local ESP32 IP: ");
    // Serial.println(WiFi.localIP());

    
}

void loop() {

if (loopCounter % 5 == 0) {
  if (loopCounter >= 100000) {
    loopCounter = 0;
  }

  lastTemp = currentTemp;
  currentTemp = thermocouple.readCelsius();

  if (currentTemp >= targetTemp) {
    dutyCycle = 0;
    fanRunningCounter = 0;
  } else {
    fanRunningCounter++;
    if (fanRunningCounter < 5) { // wait some time before turning on fan
      return;
    }

    difference = targetTemp - currentTemp;
    if (difference > 100) {
      difference = 100;
    }
    dutyCycle = ((difference - 1) / 99) * 110 /* MAX */ + 30 /* MIN */;

    if (fanRunningCounter % 10 == 0) { 
      tempDiff = olderTemp - currentTemp;
      if (tempDiff > 0.5) {
        // temp is sinking -> increase fan speed
        int increase = ((tempDiff - 0.5) / 4.5) * 50 + 10;
        dutyCycle += increase;
      } 
      olderTemp = currentTemp;
    }
  }

  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  delay(300);
}

  lcd.setCursor(0,0);
  lcd.print("Current: " + String(currentTemp) + "C");
  lcd.setCursor(0,1);
  lcd.print("Target: " + String(targetTemp) + "C");

  

  // Move DC motor forward 
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);




  ledcWrite(pwmChannel, dutyCycle);
  loopCounter++;
  delay(200);
}

bool tempIsSinking() {
  if (currentTemp < lastTemp) {
    isSinkingCounter++;
    return true;
  }
  isSinkingCounter = 0;
  return false;
}