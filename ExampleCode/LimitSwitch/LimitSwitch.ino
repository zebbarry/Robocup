/*
Test out the limit switches on the IO extension board
*/
int PinMag     = 32;
int PinLimitMag = 44;
int PinCapSense = 45;

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
 
  pinMode(PinLimitMag,INPUT);  //Define all the pins as inputs
  pinMode(PinCapSense,INPUT);  //Define all the pins as inputs
  pinMode(PinMag,OUTPUT);  //Define all the pins as inputs
  
  Serial.begin(9600);        //Set up serial communications
}

void loop()
{
  int Limit01, Cap01;
  
  //Read all limit switches
  Limit01=digitalRead(PinLimitMag);
  Cap01=digitalRead(PinCapSense);

  if (Limit01) {
    digitalWrite(PinMag, LOW);
  } else {
    digitalWrite(PinMag, HIGH);
  }
  //Print result to serial port
  Serial.print(Cap01);
  Serial.println(Limit01);
}
