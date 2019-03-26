const int  ledPin =   13;      // the number of the LED pin
const long interval = 100;     // interval at which to blink (milliseconds)

void setup()                   // this section is executed once at statup
{
  pinMode(ledPin, OUTPUT);     // set the digital pin as output:
}

void loop()                    // this section loops forever
{
    digitalWrite(ledPin, 0);   // turn port off
    delay(interval);           // time delay
    digitalWrite(ledPin, 1);   // turn port on
    delay(interval);           // time delay
}

