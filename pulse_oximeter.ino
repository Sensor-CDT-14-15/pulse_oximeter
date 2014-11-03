#include "Timer.h"

const int RED_LED_PIN=13;
const int IR_LED_PIN=7;
const int PHOTODIODE_PIN=A0;

const int FLASH_TIME=500;
const int FLASH_PAIR_TIME=4000;
const int INTERFLASH_DELAY=500;

int currentPin = RED_LED_PIN;

Timer t;

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(IR_LED_PIN, OUTPUT);

  t.every(FLASH_PAIR_TIME, flashPair);
}

void loop() {
  t.update();
}

void switchPin() {
  currentPin = (currentPin == RED_LED_PIN) ? IR_LED_PIN : RED_LED_PIN;
}

void flashPair() {
  for(int i=0; i<2; i++) {
    digitalWrite(currentPin, HIGH);
    delay(FLASH_TIME);
    digitalWrite(currentPin, LOW);
    delay(INTERFLASH_DELAY);
    switchPin();
  }
}
