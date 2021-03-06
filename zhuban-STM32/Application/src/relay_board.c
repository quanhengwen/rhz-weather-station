#include "relay_board.h"

void relay_board_control(void)
{
  unsigned char len = 0;  //长度
  unsigned char buf[50] = {0};
  
  buf[0] = 0x66;
  buf[1] = 0xBB;
  len = 3 + 6 * 5;
  buf[2] = len;
  
  buf[3] = 0x00;
  buf[4] = 0x01;  //风机
  mem_copy(&buf[5], (unsigned char*)&(wat_fer.ventilate), 4);
  
  buf[9] = 0x00;
  buf[10] = 0x03;  //卷膜
  mem_copy(&buf[11], (unsigned char*)&(wat_fer.juanmo), 4);
  
  buf[15] = 0x00;
  buf[16] = 0x05;  //水帘
  mem_copy(&buf[17], (unsigned char*)&(wat_fer.wat4), 4);
  
  buf[21] = 0x00;
  buf[22] = 0x07;  //内，遮阳1
  mem_copy(&buf[23], (unsigned char*)&(wat_fer.yang), 4);
  
  buf[27] = 0x00;
  buf[28] = 0x09;  //外，遮阳2
  mem_copy(&buf[29], (unsigned char*)&(wat_fer.yang1), 4);
  
  USART_Puts(USART3, buf, len);
}