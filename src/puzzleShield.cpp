#include "puzzleShield.h"

PuzzleShield::PuzzleShield()
  : ledStrip1(1, LED_STRIP1, NEO_GRB + NEO_KHZ800),
    ledStrip2(1, LED_STRIP2, NEO_GRB + NEO_KHZ800),
    statusLed(1, STATUS_LED, NEO_GRB + NEO_KHZ800) {

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // Initialize LED strips
  ledStrip1.begin();
  ledStrip2.begin();
  statusLed.begin();

  // Initialize DFPlayer Mini
  dfplayer.begin(DFPLAYER_RX, DFPLAYER_TX);

  // Set initial status LED color (off)
  setStatusLEDColor(0, 0, 0);
}

void PuzzleShield::begin() {
  // Set default I2C address
  setI2CAddress(0x00);
}

void PuzzleShield::selectI2C(uint8_t address) {
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << address);          // send byte to select bus
  Wire.endTransmission();
  Serial.print(address);
}

void PuzzleShield::writeI2C(uint8_t address, uint8_t data) {
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

void PuzzleShield::readI2C(uint8_t address, uint8_t* data, uint8_t length) {
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(TCA9548A_ADDR, length);
  for (uint8_t i = 0; i < length; i++) {
    data[i] = Wire.read();
  }
}

void PuzzleShield::setRelay(uint8_t relay, bool state) {
  digitalWrite(relay, state);
}

void PuzzleShield::setDigitalPin(uint8_t pin, bool state) {
  digitalWrite(pin, state);
}

void PuzzleShield::setLEDStrip(uint8_t strip, uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t color = convertRGBToColor(red, green, blue);
  if (strip == 1) {
    ledStrip1.setPixelColor(0, color);
    ledStrip1.show();
  } else if (strip == 2) {
    ledStrip2.setPixelColor(0, color);
    ledStrip2.show();
  }
}

void PuzzleShield::setDFPlayerVolume(uint8_t volume) {
  dfplayer.setVolume(volume);
}

void PuzzleShield::playDFPlayerTrack(uint8_t track) {
  dfplayer.play(track);
}

void PuzzleShield::stopDFPlayer() {
  dfplayer.stop();
}

void PuzzleShield::startPuzzle() {
  setStatusLEDColor(0, 0, 255); // Blue for started
  EscapeLogicClient::puzzleStart();
}

void PuzzleShield::solvePuzzle() {
  setStatusLEDColor(0, 255, 0); // Green for solved
  EscapeLogicClient::puzzleSolved();
}

void PuzzleShield::resetPuzzle() {
  setStatusLEDColor(255, 0, 0); // Red for reset
  EscapeLogicClient::puzzleReset();
}

void PuzzleShield::loop() {
  if (digitalRead(START_BTN) == HIGH) {
    startPuzzle();
  }
  if (digitalRead(RESET_BTN) == HIGH) {
    resetPuzzle();
  }
  if (digitalRead(SOLVE_BTN) == HIGH) {
    solvePuzzle();
  }

  EscapeLogicClient::loop();
}

void PuzzleShield::setStatusLEDColor(uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t color = convertRGBToColor(red, green, blue);
  statusLed.setPixelColor(0, color);
  statusLed.show();
}

uint32_t PuzzleShield::convertRGBToColor(uint8_t red, uint8_t green, uint8_t blue) {
  return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}
