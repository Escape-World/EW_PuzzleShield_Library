#ifndef PUZZLESHIELD_H
#define PUZZLESHIELD_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_PN532.h>

// I2C Multiplexer
#define TCA9548A_ADDR 0x70
#define I2C_SDA 20
#define I2C_SCL 21

// Relays
#define RELAY1 5
#define RELAY2 6
#define RELAY3 7

// Digital Pins
#define DIGITAL1 26
#define DIGITAL2 27
#define DIGITAL3 28
#define DIGITAL4 29
#define DIGITAL5 30
#define DIGITAL6 31
#define DIGITAL7 32
#define DIGITAL8 33
#define DIGITAL9 34
#define DIGITAL10 35

// LED Strips
#define LED_STRIP1 36
#define LED_STRIP2 37

// DFPlayer Mini
#define DFPLAYER_RX 18
#define DFPLAYER_TX 19

// Buttons
#define START_BTN 8
#define RESET_BTN 9
#define SOLVE_BTN 11

// WS2812B Status LED
#define STATUS_LED 13

// NFC Sensors
#define NFC_SCK 38
#define NFC_MOSI 39
#define NFC_MISO 40

// NFC Sensor CS Pins
#define NFC_CS1 42
#define NFC_CS2 43
#define NFC_CS3 44
#define NFC_CS4 45
#define NFC_CS5 46
#define NFC_CS6 47
#define NFC_CS7 48
#define NFC_CS8 49

class PuzzleShield {
public:
  PuzzleShield();
  void begin();
  void selectI2C(uint8_t address);
  void writeI2C(uint8_t address, uint8_t data);
  void readI2C(uint8_t address, uint8_t* data, uint8_t length);
  void setRelay(uint8_t relay, bool state);
  void setDigitalPin(uint8_t pin, bool state);
  void setLEDStrip(uint8_t strip, uint8_t red, uint8_t green, uint8_t blue);
  void setDFPlayerVolume(uint8_t volume);
  void playDFPlayerTrack(uint8_t track);
  void stopDFPlayer();
  void startPuzzle();
  void solvePuzzle();
  void resetPuzzle();
  void loop();

  // NFC Functions
  bool initializeNFC();
  bool readNFC(uint8_t sensorIndex, uint8_t *uid, uint8_t *uidLength);
  bool matchNFCUUID(uint8_t *readUUID, uint8_t readLength, uint8_t *specifiedUUID, uint8_t specifiedLength);

private:
  Adafruit_NeoPixel ledStrip1;
  Adafruit_NeoPixel ledStrip2;
  Adafruit_NeoPixel statusLed;
  DFRobotDFPlayerMini dfplayer;
  Adafruit_PN532 *nfcSensors[8];

  void setStatusLEDColor(uint8_t red, uint8_t green, uint8_t blue);
  uint32_t convertRGBToColor(uint8_t red, uint8_t green, uint8_t blue);
};

#endif
