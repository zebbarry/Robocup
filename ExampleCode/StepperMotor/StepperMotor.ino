/*
This sample code is for testing the 4 stepper motors
All motors will rotate the same direction at the same time
*/
int M1dirpin = 20;
int M1steppin = 21;

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  //Setup step and direction pins for output
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
}

void loop()
{
  int j;
  
  //Set direction for all channels
  digitalWrite(M1dirpin,LOW);
  
  for(j=0;j<=1000;j++)            //Move 1000 steps
  {
    digitalWrite(M1steppin,LOW);
    delayMicroseconds(2);
    digitalWrite(M1steppin,HIGH);
    delay(1);
  }
}
