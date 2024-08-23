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

uint8_t prevMod2;
uint8_t currentMod2;

uint16_t *selectedScale = scale_chromatic;
uint8_t scaleLength = sizeof(scale_chromatic) / sizeof(uint16_t);

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BUT_A_PIN, INPUT_PULLUP);
  pinMode(BUT_B_PIN, INPUT_PULLUP);
  pinMode(SPI_SS_PIN, OUTPUT);

  dac.begin();
}

void setScale(uint8_t scaleSelector) {
  switch (scaleSelector) {
    case 0:
      selectedScale = scale_chromatic;
      scaleLength = sizeof(scale_chromatic) / sizeof(uint16_t);
      break;
    case 1:
      selectedScale = scale_major;
      scaleLength = sizeof(scale_major) / sizeof(uint16_t);
      break;
    case 2:
      selectedScale = scale_minor;
      scaleLength = sizeof(scale_minor) / sizeof(uint16_t);
      break;
    case 3:
      selectedScale = scale_major_triad;
      scaleLength = sizeof(scale_major_triad) / sizeof(uint16_t);
      break;
    case 4:
      selectedScale = scale_minor_triad;
      scaleLength = sizeof(scale_minor_triad) / sizeof(uint16_t);
      break;
    case 5:
      selectedScale = scale_pentatonic_major;
      scaleLength = sizeof(scale_pentatonic_major) / sizeof(uint16_t);
      break;
    case 6:
      selectedScale = scale_pentatonic_minor;
      scaleLength = sizeof(scale_pentatonic_minor) / sizeof(uint16_t);
      break;
  }
}

void loop() {
  currentMod2 = map(analogRead(MOD2_PIN), 0, 1023, 0, NMR_SCALES);
  if (currentMod2 != prevMod2) {
    setScale(currentMod2);
  }

  uint16_t cvInVal = map(analogRead(MOD1_PIN), 0, 1023, 0, 4095); // input CV is 10bits (0-1023), map it to 12bits (0-4095)

  for (int i = 0; i < scaleLength; i++) {
    uint16_t semitone = pgm_read_word(&selectedScale[i]);
    uint16_t nextSemitone = pgm_read_word(&selectedScale[i + 1]);

    if (cvInVal >= semitone && cvInVal <= nextSemitone) {
      dac.analogWrite(semitone);
      break;
    }
  }

  prevMod2 = currentMod2;
}
