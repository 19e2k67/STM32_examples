#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

//������ʼ������
void KEY_Init(void) { //IO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PORTA,PORTCʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2����
//4��KEY3����
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode) {
	static u8 key_up = 1; //�������ɿ���־
	if (mode)
		key_up = 1; //֧������
	if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0)) {
		delay_ms(10);//ȥ����
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
	return 0;// �ް�������
}
