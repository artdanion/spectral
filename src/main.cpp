#include <Arduino.h>
#include <OctoWS2811.h>
#include <ArduinoJson.h>
#include "Wire.h"

#define pot_address 0x2E // each I2C object has a unique bus address, the MCP4018 is 0x2F or 0101111 in binary

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define YELLOW 0xFFFF00
#define PINK 0xFF1088
#define ORANGE 0xE05800
#define WHITE 0xFFFFFF

// Less intense...
/*
#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010
*/

// Any group of digital pins may be used
const int numPins = 25;
byte pinList[numPins] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 17, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

const int ledsPerStrip = 7;

int dt = 1000;    // used for delay duration
byte rval = 0x00; // used for value sent to potentiometer

void colorWipe(int color, int wait);
void potLoop();

// These buffers need to be large enough for all the pixels.
// The total number of pixels is "ledsPerStrip * numPins".
// Each pixel needs 3 bytes, so multiply by 3.  An "int" is
// 4 bytes, so divide by 4.  The array is created using "int"
// so the compiler will align it to 32 bit memory.
const int bytesPerLED = 3; // change to 4 if using RGBW
DMAMEM int displayMemory[ledsPerStrip * numPins * bytesPerLED / 4];
int drawingMemory[ledsPerStrip * numPins * bytesPerLED / 4];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config, numPins, pinList);

void setup()
{
  Serial1.begin(115200);
  Wire.begin();
  leds.begin();
  leds.show();

  pinMode(A0, INPUT);
  /*
  rval = 20;
  Wire.beginTransmission(pot_address);
  Wire.write(0b00000000);
  Wire.write(rval);
  Wire.endTransmission();
  Serial.print(" sent - ");
  Serial.println(rval, HEX);
  */
}

void loop()
{

  
  int microsec = 200000 / leds.numPixels(); // change them all in 0.2 seconds

  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;

  colorWipe(RED, microsec);
  colorWipe(GREEN, microsec);
  colorWipe(BLUE, microsec);
  colorWipe(YELLOW, microsec);
  colorWipe(PINK, microsec);
  colorWipe(ORANGE, microsec);

  Serial1.println(analogRead(A0));

  //potLoop();

  delay(1000);
}

void colorWipe(int color, int wait)
{
  for (int i = 0; i < leds.numPixels(); i++)
  {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}

void potLoop()
// sends values of 0x00 to 0x2E to pot in order to change the resistance
// equates to 0~120
{
  for (rval = 0; rval < 120; rval++)
  {
    Wire.beginTransmission(pot_address);
    Wire.write(0b00000000);
    Wire.write(rval); //
    Wire.endTransmission();
    Serial.print(" sent - ");
    Serial.println(rval, HEX);
    Serial.println(analogRead(14));
    delay(dt);
  }
}
