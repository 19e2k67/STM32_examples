#include "key.h"

uint8_t KEY_Scan(uint8_t mode)
{
	static uint8_t key_up=1;//按键按松开标志
	if(mode)
		key_up=1;  //支持连按
	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0))
	{
		HAL_Delay(10);//去抖动
		key_up = 0;
		if (KEY1 == 0)
			return 1;
		else if (KEY2 == 0)
			return 2;
		else if (KEY3 == 0)
			return 3;
		else if (KEY4 == 0)
			return 4;
		else if (KEY5 == 0)
			return 5;
	} else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1)
		key_up = 1;
 	return 0;// 无按键按下
}
