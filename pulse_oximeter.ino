#include "Timer.h"

const int RED_LED_PIN=13;
const int IR_LED_PIN=7;
const int PHOTODIODE_PIN=A0;

const int FLASH_TIME=500;
const int INTERFLASH_DELAY=500;
const int PHOTODIODE_READ_TIME=20;

int currentPin = RED_LED_PIN;
String currentLED = "";

Timer ledOnTimer;
Timer ledOffTimer;
Timer photodiodeTimer;

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(IR_LED_PIN, OUTPUT);

  Serial.begin(57600);

  photodiodeTimer.every(PHOTODIODE_READ_TIME, readDiode);

  flashLEDOn();
}

void loop() {
  ledOnTimer.update();
  ledOffTimer.update();
  photodiodeTimer.update();
}

void switchPin() {
  currentPin = (currentPin == RED_LED_PIN) ? IR_LED_PIN : RED_LED_PIN;
}

void flashLEDOn() {
  digitalWrite(currentPin, HIGH);
  ledOnTimer.after(FLASH_TIME, flashLEDOff);
  currentLED = (currentPin == RED_LED_PIN) ? " RED ON" : " IR ON";
  Serial.print(millis());
  Serial.println(currentLED);
}

void flashLEDOff() {
  digitalWrite(currentPin, LOW);
  ledOffTimer.after(FLASH_TIME + INTERFLASH_DELAY, flashLEDOn);
  currentLED = (currentPin == RED_LED_PIN) ? " RED OFF" : " IR OFF";
  switchPin();
  Serial.print(millis());
  Serial.println(currentLED);
}

void readDiode() {
  Serial.print(millis());
  Serial.print(" PHOTODIODE ");
  Serial.println(analogRead(PHOTODIODE_PIN));
}
