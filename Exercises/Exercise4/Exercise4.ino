#include "Adafruit_NeoPixel.h"
#include <avr/power.h>

#define PIN            2   // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS      10  // How many NeoPixels are attached to the Arduino?

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;       // delay for half a second

void setup() 
{
  pixels.begin();         // This initializes the NeoPixel library.
}

void loop() 
{
  for(int i=0;i<NUMPIXELS;i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,25));         // Moderately bright green color.
    pixels.show();                                         // This sends the updated pixel color to the hardware.
    delay(delayval);                                       // Delay for a period of time (in milliseconds).
  }
}
