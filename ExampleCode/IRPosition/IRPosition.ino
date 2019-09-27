// Wii Remote IR sensor  test sample code  by kako http://www.kako.com
// modified output for Wii-BlobTrack program by RobotFreak http://www.letsmakerobots.com/user/1433
// modified for http://DFRobot.com by Lumi, Jan. 2014

#include <Wire.h>
#include "irposition.h"


int cam_x[4];
int cam_y[4];

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board

  Serial.begin(19200);
  Wire.begin();
  cam_init();
}

void loop()
{
  read_cam();
  delay(15);
}
