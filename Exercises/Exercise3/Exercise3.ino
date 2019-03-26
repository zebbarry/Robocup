const int LED_Red =      2;        // the number of the LED pin
const int LED_Yellow =   3; 
const int LED_Green =    4; 
const long interval   =  2000;     // timeing interval

void setup() 
{
  pinMode(LED_Red, OUTPUT);       // set the digital pin as output
  pinMode(LED_Yellow, OUTPUT); 
  pinMode(LED_Green, OUTPUT); 
}

void Traffic(int Red,int Yellow,int Green)  //Function to set color od LED's
{
   digitalWrite(LED_Red, Red);   
   digitalWrite(LED_Yellow, Yellow);
   digitalWrite(LED_Green, Green);              
}

void loop()
{
  Traffic(1,0,0);                //Set color
  delay(interval);               //Time delay
  Traffic(0,1,0);
  delay(interval);
  Traffic(0,0,1);
  delay(interval);
}



