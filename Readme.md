## PuzzleShield Library
The PuzzleShield library is a comprehensive Arduino library that provides an easy-to-use interface for controlling the PuzzleShield hardware module. The library includes functions for controlling I2C devices, digital pins, LED strips, and the DFPlayer Mini MP3 player.

## Installation

To install the PuzzleShield library, simply download the ZIP file from the GitHub repository and extract it to your Arduino libraries folder. You can also install the library using the Arduino Library Manager by searching for "PuzzleShield".

## Usage

To use the PuzzleShield library, first create an instance of the `PuzzleShield` class. Then, you can use the following functions to control the hardware module:

* `begin()` - Initializes the I2C bus and the WS2801 LED driver.
* `setI2CAddress(uint8_t address)` - Sets the I2C address of the PuzzleShield module.
* `writeI2C(uint8_t address, uint8_t data)` - Writes a single byte of data to the specified I2C address.
* `readI2C(uint8_t address, uint8_t* data, uint8_t length)` - Reads multiple bytes of data from the specified I2C address.
* `setRelay(uint8_t relay, bool state)` - Sets the state of the specified relay.
* `setDigitalPin(uint8_t pin, bool state)` - Sets the state of the specified digital pin.
* `setLEDStrip(uint8_t strip, uint32_t color)` - Sets the color of the specified LED strip.
* `setDFPlayerVolume(uint8_t volume)` - Sets the volume of the DFPlayer Mini MP3 player.
* `playDFPlayerTrack(uint8_t track)` - Plays the specified track on the DFPlayer Mini MP3 player.
* `stopDFPlayer()` - Stops playback on the DFPlayer Mini MP3 player.

## Example

The following example shows how to use the PuzzleShield library to control an LED strip and a relay:

```cpp
#include "puzzleShield.h"

PuzzleShield puzzleShield;

void setup() {
  puzzleShield.begin();

  // Set the I2C address of the PuzzleShield module
  puzzleShield.setI2CAddress(0x00);

  // Set the color of the LED strip
  puzzleShield.setLEDStrip(1, 0xFF0000);

  // Set the state of the relay
  puzzleShield.setRelay(1, true);
}

void loop() {
  // Do something...
}
