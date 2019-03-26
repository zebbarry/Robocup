#include "IIC_without_ACK.h"
#include "oledfont.c"   

#define OLED_SDA A4
#define OLED_SCL A5

IIC_without_ACK oled(OLED_SDA, OLED_SCL);

void setup()
{
  oled.Initial();
  delay(10);
}

void loop()
{
  oled.Fill_Screen(0x00);
  oled.Char_F6x8(0,0,"Mechatronics");
  oled.Char_F6x8(0,2,"University Canterbury");
  oled.Char_F6x8(0,4,"Christchurch");
  oled.Char_F6x8(0,6,"New Zealand");
  delay(5000);
}
