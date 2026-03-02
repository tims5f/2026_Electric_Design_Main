#include "user.h"
#include "gpio.h"
#include "oled.h"

/*二极管档
直流电压20
直流电压2
交流电压20
交流电压2
频率20
频率2
蜂鸣档
电阻档4个  200 2k 20k 200k*/

void Mode_show()
{
  OLED_Clear();

  OLED_ShowChinese(0, 17, 24, 16);
  OLED_ShowChinese(17, 17, 25, 16);
  OLED_ShowChar(34,17,58,16,1);
  
  if(mode == 255)
  {
    //OLED_Clear();
    OLED_ShowChar(0,0,50,16,1);
    OLED_ShowChar(0,0,48,16,1);
  }

  if(mode == 0)    //二极管档
  {
    OLED_ShowChinese(0, 0, 2, 16);
    OLED_ShowChinese(17, 0, 3, 16);
    OLED_ShowChinese(33, 0, 4, 16);
    OLED_ShowChinese(49, 0, 5, 16);
  }

  if(mode == 1)   //直流电压档20V，2V
  {
    OLED_ShowChinese(0, 0, 15, 16);
    OLED_ShowChinese(17, 0, 16, 16);
    OLED_ShowChinese(33, 0, 6, 16);
    OLED_ShowChinese(49, 0, 7, 16);
    if(v_mode == 0)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,48,16,1);
      OLED_ShowChar(83,0,86,16,1);
      OLED_ShowChinese(91, 0, 5, 16);

      OLED_ShowChar(43,17,86,16,1);
      
    }else if(v_mode == 1)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,86,16,1);
      OLED_ShowChinese(83, 0, 5, 16);

      OLED_ShowChar(43,17,109,16,1);
      OLED_ShowChar(52,17,86,16,1);
    }
  }

  if(mode == 2)    //交流电压档20V，2V
  {
    OLED_ShowChinese(0, 0, 17, 16);
    OLED_ShowChinese(17, 0, 18, 16);
    OLED_ShowChinese(33, 0, 6, 16);
    OLED_ShowChinese(49, 0, 7, 16);
    if(v_mode == 0)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,48,16,1);
      OLED_ShowChar(83,0,86,16,1);
      OLED_ShowChinese(91, 0, 5, 16);

      OLED_ShowChar(43,17,86,16,1);
    }else if(v_mode == 1)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,86,16,1);
      OLED_ShowChinese(83, 0, 5, 16);

      OLED_ShowChar(43,17,109,16,1);
      OLED_ShowChar(52,17,86,16,1);
    }
  }

  if(mode == 3)    //电阻档4个
  {
    OLED_ShowChinese(0, 0, 9, 16);
    OLED_ShowChinese(17, 0, 10, 16);
    if(r_mode == 0)
    {
      OLED_ShowChar(33,0,50,16,1);
      OLED_ShowChar(42,0,48,16,1);
      OLED_ShowChar(51,0,48,16,1);
      OLED_ShowChinese(60, 0, 23, 16);
      OLED_ShowChinese(77, 0, 11, 16);
      OLED_ShowChinese(43, 17, 23, 16);
    }
    if(r_mode == 1)
    {
      OLED_ShowChar(33,0,50,16,1);
      OLED_ShowChar(42,0,107,16,1);
      OLED_ShowChinese(51, 0, 23, 16);
      OLED_ShowChinese(68, 0, 11, 16);
      OLED_ShowChar(43,17,107,16,1);
      OLED_ShowChinese(52, 17, 23, 16);
    }
    if(r_mode == 2)
    {
      OLED_ShowChar(33,0,50,16,1);
      OLED_ShowChar(42,0,48,16,1);
      OLED_ShowChar(51,0,107,16,1);
      OLED_ShowChinese(60, 0, 23, 16);
      OLED_ShowChinese(77, 0, 11, 16);
      OLED_ShowChar(43,17,107,16,1);
      OLED_ShowChinese(52, 17, 23, 16);
    }
    if(r_mode == 3)
    {
      OLED_ShowChar(33,0,50,16,1);
      OLED_ShowChar(42,0,48,16,1);
      OLED_ShowChar(51,0,48,16,1);
      OLED_ShowChar(60,0,107,16,1);
      OLED_ShowChinese(67, 0, 23, 16);
      OLED_ShowChinese(83, 0, 11, 16);
      OLED_ShowChar(43,17,107,16,1);
      OLED_ShowChinese(52, 17, 23, 16);
    }
  }

  if(mode == 4)    //蜂鸣档
  {
    OLED_Clear();
    OLED_ShowChinese(0, 0, 12, 16);
    OLED_ShowChinese(17, 0, 13, 16);
    OLED_ShowChinese(33, 0, 14, 16);
  }

  if(mode == 5)    //频率测量
  {
    OLED_ShowChinese(0, 0, 19, 16);
    OLED_ShowChinese(17, 0, 20, 16);
    OLED_ShowChinese(33, 0, 21, 16);
    OLED_ShowChinese(49, 0, 22, 16);
    if(v_mode == 0)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,48,16,1);
      OLED_ShowChar(83,0,86,16,1);
    }else if(v_mode == 1)
    {
      OLED_ShowChar(65,0,50,16,1);
      OLED_ShowChar(74,0,86,16,1);
    }
  }

  OLED_Refresh_Gram();
}
