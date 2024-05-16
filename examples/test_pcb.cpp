#include <Arduino.h>
#include "puzzleShield.h"
#include "EscapeLogic.hpp"


#define DEVICE_NAME "puzzle1"
byte mac[] = {0xDE, 0xAB, 0xCE, 0xFE, 0xFE, 0xFA}; // Change to a random MAC address.
IPAddress server(192, 168, 68, 148);			   // Server IP address

PuzzleShield puzzleShield;

void puzzleStart()
{
	EscapeLogicClient::log("Puzzle start..");
	Serial.println("Puzzle Start");
}

void resetEverything()
{
	EscapeLogicClient::log("Reset everything..");
}

void puzzleLoop()
{
    Serial.println("Puzzle Loop");
}

void puzzleSolved()
{

	EscapeLogicClient::log("Puzzle finished.");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Puzzle Shield");

  puzzleShield.begin();

  Serial.println("Puzzle Shield started");

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  pinMode(START_BTN, INPUT);
  pinMode(RESET_BTN, INPUT);
  pinMode(SOLVE_BTN, INPUT);

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

  // EscapeLogicClient::start(mac, server, DEVICE_NAME);
	// EscapeLogicClient::setOnPuzzleStart(puzzleStart);
	// EscapeLogicClient::setPuzzleLoop(puzzleLoop);
	// EscapeLogicClient::setOnPuzzleSolved(puzzleSolved);
	// EscapeLogicClient::setOnReset(resetEverything);
}

void loop() {
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
  // EscapeLogicClient::loop();
}
