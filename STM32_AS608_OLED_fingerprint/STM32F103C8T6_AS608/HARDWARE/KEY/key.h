#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"	 


#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����2
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����3 
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����4
#define KEY5  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����5

extern u8 key_num;

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);//����ɨ�躯��					    
#endif
