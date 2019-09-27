/*
Test out the limit switches on the IO extension board
*/
int PinLimit01 = 45;

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
 
  pinMode(PinLimit01,INPUT);  //Define all the pins as inputs
  
  Serial.begin(9600);        //Set up serial communications
}

void loop()
{
  int Limit01;
  
  //Read all limit switches
  Limit01=digitalRead(PinLimit01);
  
  //Print result to serial port
  Serial.print(Limit01);
}
