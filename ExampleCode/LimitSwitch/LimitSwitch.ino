/*
  Test out the limit switches on the IO extension board
*/
//#include <Herkulex.h>
//int PinMag     = 32;
//int PinLimitMag = 44;
int PinCapSense = 45;
int red = 42;
int blue = 43;
int green = 44;
int n = 0xfe; //motor ID - verify your ID !!!!

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board

//  Herkulex.beginSerial2(115200); //open serial port 2 to talk to the motors
//  Herkulex.reboot(n);            //reboot motor
//  delay(500);
//  Herkulex.initialize();         //initialize motors
//  delay(200);

//  pinMode(PinMag, OUTPUT); //Define all the pins as inputs
//  pinMode(PinLimitMag, INPUT); //Define all the pins as inputs
  pinMode(PinCapSense, INPUT_PULLUP); //Define all the pins as inputs
  pinMode(red, OUTPUT); //Define all the pins as inputs
  pinMode(blue, OUTPUT); //Define all the pins as inputs
  pinMode(green, OUTPUT); //Define all the pins as inputs
//  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
  digitalWrite(green, HIGH);
  
  Serial.begin(9600);        //Set up serial communications
}

void loop()
{
  int Limit01, Cap01;

  //Read all limit switches
//  Limit01 = digitalRead(PinLimitMag);
  Cap01 = digitalRead(PinCapSense);

//  if (Cap01) {
//    digitalWrite(PinMag, LOW);
//    //    Herkulex.setLed(n, LED_BLUE);
//  } else {
//    digitalWrite(PinMag, HIGH);
//    //    Herkulex.moveOneAngle(n, 100, 1000, LED_GREEN); //move motor forward
//  }

  //Print result to serial port
  Serial.print(Cap01);
//  Serial.println(Limit01);
}
