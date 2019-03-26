
const int trigPin = 12;
const int echoPin = 11;
 
void setup() 
{
// initialize serial communication:
Serial.begin(9600);
}
 
void loop()
{

long duration, cm;
 
// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
pinMode(trigPin, OUTPUT);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
 
// Read the signal from the sensor: a HIGH pulse whose
// duration is the time (in microseconds) from the sending
// of the ping to the reception of its echo off of an object.
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
 
// convert the time into a distance
cm = microsecondsToCentimeters(duration);
Serial.print(cm);
Serial.println();
delay(100);
}
 
 
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
} 
