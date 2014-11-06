#include "Timer.h"

const int RED_LED_PIN=13;
const int IR_LED_PIN=7;
const int PHOTODIODE_PIN=A0;

const int FLASH_TIME=500;
const int INTERFLASH_DELAY=500;
const int PHOTODIODE_READ_TIME=20;

int currentPin = RED_LED_PIN;
String currentLED = "";

Timer t;

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(IR_LED_PIN, OUTPUT);

  Serial.begin(9600);

  t.every(PHOTODIODE_READ_TIME, readDiode);

  flashLEDOn();
}

void loop() {
  t.update();
}

void switchPin() {
  currentPin = (currentPin == RED_LED_PIN) ? IR_LED_PIN : RED_LED_PIN;
}

void flashLEDOn() {
  digitalWrite(currentPin, HIGH);
  t.after(FLASH_TIME, flashLEDOff);
  currentLED = (currentPin == RED_LED_PIN) ? " RED LED ON" : " IR LED ON";
  Serial.print(millis());
  Serial.println(currentLED);
}

void flashLEDOff() {
  digitalWrite(currentPin, LOW);
  t.after(FLASH_TIME + INTERFLASH_DELAY, flashLEDOn);
  currentLED = (currentPin == RED_LED_PIN) ? " RED LED OFF" : " IR LED OFF";
  switchPin();
  Serial.print(millis());
  Serial.println(currentLED);
}

void readDiode() {
  Serial.print(millis());
  Serial.print(" PHOTODIODE LEVEL ");
  Serial.println(analogRead(PHOTODIODE_PIN));
}
