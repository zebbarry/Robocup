const int DigitalInPin       = 12;
const int DigitalOutPinHigh   = 2;
const int DigitalOutPinLow     = 3;

void setup() 
{
  pinMode(DigitalInPin, INPUT);
  pinMode(DigitalOutPinHigh, OUTPUT);
  pinMode(DigitalOutPinLow, OUTPUT);
}

void loop() 
{
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(DigitalInPin);

  if (sensorVal == HIGH) 
  {
    digitalWrite(DigitalOutPinHigh, HIGH);
    digitalWrite(DigitalOutPinLow, LOW);
  }
  else 
  {
    digitalWrite(DigitalOutPinHigh, LOW);
    digitalWrite(DigitalOutPinLow, HIGH);
  }
}



