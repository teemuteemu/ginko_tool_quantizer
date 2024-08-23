#include <Arduino.h>
#include <avr/pgmspace.h>

#include "MCP492X.h"

#include "scale.h"

#define MOD1_PIN 		A1
#define MOD2_PIN 		A2
#define TRIGGER_PIN 4
#define BUT_A_PIN 2
#define BUT_B_PIN 9
#define LED1_PIN 		3
#define LED2_PIN 		5
#define SPI_SS_PIN 10

MCP492X dac(SPI_SS_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BUT_A_PIN, INPUT_PULLUP);
  pinMode(BUT_B_PIN, INPUT_PULLUP);
  pinMode(SPI_SS_PIN, OUTPUT);

  dac.begin();
}

void loop() {
  uint16_t cvInVal = map(analogRead(MOD1_PIN), 0, 1023, 0, 4095); // input CV is 10bits (0-1023), map it to 12bits (0-4095)

  for (int i = 0; i < NMR_SEMINOTES; i++) {
    uint16_t semitone = pgm_read_word(&semitones[i]);
    uint16_t nextSemitone = pgm_read_word(&semitones[i + 1]);

    if (cvInVal >= semitone && cvInVal <= nextSemitone) {
      dac.analogWrite(semitone);
      break;
    }
  }
}
