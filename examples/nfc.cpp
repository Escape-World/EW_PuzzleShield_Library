#include <puzzleShield.h>

#define NUM_LEDS_STRIP_1 0
#define NUM_LEDS_STRIP_2 0

PuzzleShield puzzleShield(NUM_LEDS_STRIP_1, NUM_LEDS_STRIP_2);

uint8_t specifiedUUID[] = {0x04, 0x25, 0x85, 0x64, 0xE0, 0x27, 0x80}; // Example UUID to match

void setup() {
  Serial.begin(115200);
  puzzleShield.begin();

  Serial.println("PuzzleShield initialized.");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;

  for (uint8_t i = 0; i < 8; i++) {
    if (puzzleShield.readNFC(i, uid, &uidLength)) {
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" read UUID: ");
      for (uint8_t j = 0; j < uidLength; j++) {
        Serial.print(uid[j], HEX);
        Serial.print(" ");
      }
      Serial.println();

      if (puzzleShield.matchNFCUUID(uid, uidLength, specifiedUUID, sizeof(specifiedUUID))) {
        Serial.println("UUID matched!");
        // Add your code here to handle a successful match
      } else {
        Serial.println("UUID did not match.");
      }
    }
  }

  delay(1000);
}
