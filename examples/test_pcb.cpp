#include <Arduino.h>
#include "puzzleShield.h"

#define DEVICE_NAME "puzzle1"
byte mac[] = {0xDE, 0xAB, 0xCE, 0xFE, 0xFE, 0xFA}; // Change to a random MAC address.
IPAddress server(192, 168, 68, 148);			   // Server IP address

#define LED_COUNT1 30  // Number of LEDs in the first strip
#define LED_COUNT2 30  // Number of LEDs in the second strip

PuzzleShield puzzleShield(LED_COUNT1, LED_COUNT2);

void puzzleStart()
{
	EscapeLogicClient::log("Puzzle start..");
	Serial.println("Puzzle Start");
}

// Called when the server wants every device to reset.
void resetEverything()
{
	EscapeLogicClient::log("Reset everything..");
}

// Loop function, called repeatedly, like void loop(),
// but only when in puzzle mode.
void puzzleLoop()
{
    Serial.println("Puzzle Loop");
}

// Called when the puzzle is solved.
void puzzleSolved()
{

	EscapeLogicClient::log("Puzzle finished.");
}

void setup() {
  Serial.begin(9600);

  puzzleShield.begin();

  // Set relay pins as outputs
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  // Set button pins as inputs
  pinMode(START_BTN, INPUT);
  pinMode(RESET_BTN, INPUT);
  pinMode(SOLVE_BTN, INPUT);

  // Print I2C devices connected to the multiplexer
  Serial.println("Scanning I2C devices...");
  for (uint8_t i = 0; i < 8; i++) {
    puzzleShield.selectI2C(i);
    Wire.beginTransmission(TCA9548A_ADDR);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device found at channel: ");
      Serial.println(i);
    }
  }
  Serial.println("Scan complete.");
}

void loop() {
  // Loop colors on LED strips
  for (uint8_t r = 0; r < 255; r++) {
    for (uint8_t g = 0; g < 255; g++) {
      for (uint8_t b = 0; b < 255; b++) {
        puzzleShield.setLEDStrip(1, r, g, b);
        puzzleShield.setLEDStrip(2, 255 - r, 255 - g, 255 - b);
        delay(10);
      }
    }
  }

  // Toggle relays
  puzzleShield.setRelay(RELAY1, HIGH);
  delay(500);
  puzzleShield.setRelay(RELAY1, LOW);
  delay(500);

  puzzleShield.setRelay(RELAY2, HIGH);
  delay(500);
  puzzleShield.setRelay(RELAY2, LOW);
  delay(500);

  puzzleShield.setRelay(RELAY3, HIGH);
  delay(500);
  puzzleShield.setRelay(RELAY3, LOW);
  delay(500);

  puzzleShield.loop();
}
