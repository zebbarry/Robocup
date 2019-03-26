const int  ledPin =  2;        // the number of the LED pin
const long interval   = 100;   // interval at which to blink (milliseconds)

void setup() 
{
  pinMode(ledPin, OUTPUT);     // set the digital pin as output:
}

void loop()
{
    digitalWrite(ledPin, 0);   // turn port off
    delay(interval);
    digitalWrite(ledPin, 1);   // turn port on
    delay(interval);
}

