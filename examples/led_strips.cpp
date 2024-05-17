#include "puzzleShield.h"

#define NUM_LEDS_STRIP_1 60
#define NUM_LEDS_STRIP_2 120

PuzzleShield puzzleShield(NUM_LEDS_STRIP_1, NUM_LEDS_STRIP_2); // Example with 60 LEDs on strip 1 and 120 LEDs on strip 2

void setup() {
  Serial.begin(9600);
  puzzleShield.begin();

  // Set initial colors for the LED strips
  puzzleShield.setLEDStrip(1, 255, 0, 0); // Red on strip 1
  puzzleShield.setLEDStrip(2, 0, 255, 0); // Green on strip 2
}

void loop() {
  // Example of changing colors every second
  puzzleShield.setLEDStrip(1, 255, 255, 0); // Yellow on strip 1
  delay(1000);
  puzzleShield.setLEDStrip(2, 0, 0, 255); // Blue on strip 2
  delay(1000);

  puzzleShield.loop(); // Check for button presses and handle them
}
