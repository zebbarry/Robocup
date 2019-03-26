/***************************************************************************
* ��Ϊ�˰汾��IICͨ��û��ACK�źŷ��أ������ֳɵĹ���IICͨ�ŵĿⲻ�����ڱ�Ʒ��
*
* ��һ��ʹ��Arduino��Ҳ��̫��ϤC++�������������ʹ���������ܲ���Adafruit��
* U8glib�Ŀ��������㡭��������ꡣ�������������С����ʾ����ߴ�BMPͼƬ�ȹ��ܣ�
* ���������ƹ��ܵģ�������
*
* ���°汾�ġ�IIC_wtihout_ACK���ᷢ���� http://www.14blog.com/archives/1358
* 
* ��������ⷴ�����뷢�ʼ��� hello14blog@gmail.com
***************************************************************************/

#ifndef _IIC_without_ACK_H
#define _IIC_without_ACK_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Adafruit_GFX.h"

#define BLACK 0
#define WHITE 1

class IIC_without_ACK
{
 public:
  IIC_without_ACK(int sda, int scl);
  
  void IIC_Start();
  void IIC_Stop();
  void Write_IIC_Byte(unsigned char IIC_Byte);
  void Write_IIC_Command(unsigned char IIC_Command);
  void Begin_IIC_Data();
  void IIC_SetPos(unsigned char x, unsigned char y);
  void Fill_Screen(unsigned char fill_Data);
  void Char_F6x8(unsigned char x, unsigned char y, const char ch[]);
//  void Char_F8x16(unsigned char x, unsigned char y,const char ch[]);
//  void CN_F16x16(unsigned char x, unsigned char y, unsigned char N);
  void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const char BMP[]);
  void Initial();
  
  private:
  int _sda, _scl;
};
#endif
