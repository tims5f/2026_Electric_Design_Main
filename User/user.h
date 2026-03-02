#pragma once




#define DEBOUNCE_TIME     0     // 消抖20ms
#define LONG_PRESS_TIME   300    // 长按1秒

typedef enum
{
    KEY_IDLE = 0,
    KEY_PRESS,
    KEY_LONG
}KeyState_t;




void Mode_show();