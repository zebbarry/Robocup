/*
This sample code is for testing the 4 stepper motors
All motors will rotate the same direction at the same time
*/
int M1dirpin = 27;
int M1steppin = 28;

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  Serial.begin(9600);
  
  //Setup step and direction pins for output
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  
  //Set direction for all channels
  digitalWrite(M1dirpin,HIGH);
  
  driveStep(2000, M1steppin);
  delay(200);
}

void driveStep(int steps, int stepPin) {
  int i = 0;
  for (i = 0; i < steps; i++) {
    digitalWrite(stepPin,LOW);
    delayMicroseconds(2);
    digitalWrite(stepPin,HIGH);
    delay(1);
  }
}

void loop()
{
  
  Serial.println("Sup");
}
