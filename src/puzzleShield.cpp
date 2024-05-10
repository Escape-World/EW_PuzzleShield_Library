#include "puzzleShield.h"

PuzzleShield::PuzzleShield() {
  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // Initialize WS2801
  ws2801.begin();

  // Initialize LED strips
  ledStrip1.begin();
  ledStrip2.begin();

  // Initialize DFPlayer Mini
  dfplayer.begin(DFPLAYER_RX, DFPLAYER_TX);
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

void PuzzleShield::setLEDStrip(uint8_t strip, uint32_t color) {
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
