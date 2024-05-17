#include "puzzleShield.h"

PuzzleShield::PuzzleShield(uint16_t numLedsStrip1, uint16_t numLedsStrip2, uint16_t numStatusLeds)
  : ledStrip1(numLedsStrip1, LED_STRIP1, NEO_GRB + NEO_KHZ800),
    ledStrip2(numLedsStrip2, LED_STRIP2, NEO_GRB + NEO_KHZ800),
    statusLed(numStatusLeds, STATUS_LED, NEO_GRB + NEO_KHZ800) {
  for (int i = 0; i < 8; i++) {
    nfcSensors[i] = nullptr;
  }
}

void PuzzleShield::begin() {
  // Set default I2C address
  selectI2C(0x00);

  // Initialize pins
  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);
  pinMode(SOLVE_BTN, INPUT_PULLUP);
  pinMode(LED_STRIP1, OUTPUT);
  pinMode(LED_STRIP2, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Initialize I2C
  Wire.begin();
  Wire.setClock(400000);

  // Initialize LED strips
  ledStrip1.begin();
  ledStrip2.begin();
  statusLed.begin();

  // Initialize DFPlayer Mini
  #define FPSerial Serial1
  FPSerial.begin(9600);
  if (!dfplayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin DFPlayer:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  } else {
    Serial.println(F("DFPlayer Mini online."));
  }
  
  dfplayer.setTimeOut(500);

  // Set initial status LED color (off)
  setStatusLEDColor(0, 0, 0);

  // Initialize NFC sensors
  initializeNFC();
}

bool PuzzleShield::initializeNFC() {
  uint8_t csPins[] = {NFC_CS1, NFC_CS2, NFC_CS3, NFC_CS4, NFC_CS5, NFC_CS6, NFC_CS7, NFC_CS8};

  for (int i = 0; i < 8; i++) {
    nfcSensors[i] = new Adafruit_PN532(NFC_SCK, NFC_MISO, NFC_MOSI, csPins[i]);
    nfcSensors[i]->begin();
    uint32_t versiondata = nfcSensors[i]->getFirmwareVersion();
    if (!versiondata) {
      Serial.print(F("Didn't find PN53x board #"));
      Serial.println(i + 1);
      return false;
    }

    nfcSensors[i]->SAMConfig();
  }
  return true;
}

bool PuzzleShield::readNFC(uint8_t sensorIndex, uint8_t *uid, uint8_t *uidLength) {
  if (sensorIndex >= 8 || nfcSensors[sensorIndex] == nullptr) {
    return false;
  }

  if (nfcSensors[sensorIndex]->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength)) {
    return true;
  } else {
    return false;
  }
}

bool PuzzleShield::matchNFCUUID(uint8_t *readUUID, uint8_t readLength, uint8_t *specifiedUUID, uint8_t specifiedLength) {
  if (readLength != specifiedLength) {
    return false;
  }

  for (uint8_t i = 0; i < readLength; i++) {
    if (readUUID[i] != specifiedUUID[i]) {
      return false;
    }
  }
  
  return true;
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
    for (uint16_t i = 0; i < ledStrip1.numPixels(); i++) {
      ledStrip1.setPixelColor(i, color);
    }
    ledStrip1.show();
  } else if (strip == 2) {
    for (uint16_t i = 0; i < ledStrip2.numPixels(); i++) {
      ledStrip2.setPixelColor(i, color);
    }
    ledStrip2.show();
  }
}

void PuzzleShield::setDFPlayerVolume(uint8_t volume) {
  dfplayer.volume(volume);
}

void PuzzleShield::playDFPlayerTrack(uint8_t track) {
  dfplayer.play(track);
}

void PuzzleShield::stopDFPlayer() {
  dfplayer.stop();
}

void PuzzleShield::startPuzzle() {
  setStatusLEDColor(0, 0, 255); // Blue for started
}

void PuzzleShield::solvePuzzle() {
  setStatusLEDColor(0, 255, 0); // Green for solved
}

void PuzzleShield::resetPuzzle() {
  setStatusLEDColor(255, 0, 0); // Red for reset
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
}

void PuzzleShield::setStatusLEDColor(uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t color = convertRGBToColor(red, green, blue);
  statusLed.setPixelColor(0, color);
  statusLed.show();
}

uint32_t PuzzleShield::convertRGBToColor(uint8_t red, uint8_t green, uint8_t blue) {
  return ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}
