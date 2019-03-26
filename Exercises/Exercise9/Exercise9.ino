#include "Adafruit_NeoPixel.h"
#include <avr/power.h>

#define PIN            2   // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS      10  // How many NeoPixels are attached to the Arduino?

const int analogInPinX = A0;  // Analog input pin that the potentiometer is attached t
const int analogInPinY = A1;  // Analog input pin that the potentiometer is attached t
const int analogInPinZ = A2;  // Analog input pin that the potentiometer is attached t
int sensorValueX = 0;        // value read from the pot
int sensorValueY = 0; 
int sensorValueZ = 0; 

int outputValueX = 0;
int outputValueY = 0;
int outputValueZ = 0;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() 
{
  pinMode(analogInPinX, INPUT);
  pinMode(analogInPinY, INPUT);
  pinMode(analogInPinZ, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() 
{
  sensorValueX = analogRead(analogInPinX);
  sensorValueY = analogRead(analogInPinY);
  sensorValueZ = analogRead(analogInPinZ);
  
  outputValueX = map(sensorValueX, 0, 1023, 0, 100);
  outputValueY = map(sensorValueY, 0, 1023, 0, 100);
  outputValueZ = map(sensorValueZ, 0, 1023, 0, 100);
  
  
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.


    pixels.setPixelColor(0, pixels.Color(outputValueX,0,0));     
    pixels.setPixelColor(1, pixels.Color(0,outputValueY,0));  
    pixels.setPixelColor(2, pixels.Color(0,0,outputValueZ));  
    pixels.show();                                         // This sends the updated pixel color to the hardware.
    delay(delayval);                                       // Delay for a period of time (in milliseconds).

}
