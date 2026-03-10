#ifndef __OLED_H
#define __OLED_H 

#include "stdlib.h"	

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

#define OLED_ADDRESS 0x78



void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1);
void OLED_ClearPoint(unsigned char x,unsigned char y);
void OLED_ColorTurn(unsigned char i);
void OLED_DisplayTurn(unsigned char i);
void Send_Byte(unsigned char dat);
void OLED_WR_Byte(unsigned char dat,unsigned char mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size,unsigned char mode);
void OLED_ShowString(unsigned char x,unsigned char y,const unsigned char *p,unsigned char size);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1);
void OLED_WR_BP(unsigned char x,unsigned char y);
void OLED_Init(void);
void Oled_Base_Show(void);
void OLED_Refresh_Gram(void);
void OLED_Clear_area(void);
void show_one_decimal(unsigned char x, unsigned char y,float value,  unsigned char size);
void show_data(int key);
void show_float(unsigned char x,
                unsigned char y,
                float value,
                unsigned char size,
                unsigned char decimal_places);
void OLED_Fill(unsigned char x1,
               unsigned char y1,
               unsigned char x2,
               unsigned char y2,
               unsigned char dot);

#endif


