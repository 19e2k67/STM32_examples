#include "sys.h"
#include "delay.h"
#include "oled_iic.h"
#include "key.h"
#include "as608.h"
#include "usart3.h"
#include "timer.h"
/*  
AS608
3.3V-->3.3V
TX-->PB11
RX-->PB10
GND-->GND

OLED
GND-->GND
VCC-->3.3��5V
SCL-->PB8
SDA-->PB9

KEY1-->PA1
KEY2-->PA2
KEY3-->PA3
KEY4-->PA4
KEY5-->PA5
*/
void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��
void ShowErrMessage(u8 ensure);//��ʾȷ���������Ϣ

u8 key_num = 0;

int main(void) {

	delay_init();
	usart3_init(57600);
	KEY_Init();
	OLED_Init();
	delay_ms(8);
	OLED_Clear();

	while (PS_HandShake(&AS608Addr)) { //��AS608ģ������
		delay_ms(400);
		OLED_ShowString(1, 1, "CONNECT FAILED");
		delay_ms(800);
		OLED_ShowString(1, 1, "CONNECTING    ");
	}
	OLED_Clear();
	OLED_ShowString(1, 1, "TEST1");
	OLED_ShowString(2, 1, "K1 ADD");
	OLED_ShowString(3, 1, "K3 DEL");
	OLED_ShowString(4, 1, "K5 VER");
	while (1) {
		key_num = KEY_Scan(0);
		if (key_num == 1) {
			key_num = 0;
			OLED_Clear();
			Add_FR();
		}
		if (key_num == 3) {
			key_num = 0;
			OLED_Clear();
			Del_FR();
		}
		if (key_num == 5) {
			key_num = 0;
			OLED_Clear();
			OLED_ShowString(1, 1, "please press");
			OLED_ShowString(2, 1, "K1 back");
			press_FR();
		}
	}
}

void ShowErrMessage(u8 ensure) {

}

//¼ָ��
void Add_FR(void) {
	u8 i, ensure, processnum = 0;
	u8 ID_NUM = 0;
	while (1) {
		switch (processnum) {
			case 0:
				i++;
				OLED_ShowString(1, 1, "    PRESS    ");
				ensure = PS_GetImage();
				if (ensure == 0x00) {
					ensure = PS_GenChar(CharBuffer1); //��������
					if (ensure == 0x00) {
						OLED_ShowString(1, 1, " Identified  ");
						i = 0;
						processnum = 1; //�����ڶ���
					} else
						ShowErrMessage(ensure);
				} else
					ShowErrMessage(ensure);
				break;

			case 1:
				i++;
				OLED_ShowString(1, 1, "   Press again   ");
				ensure = PS_GetImage();
				if (ensure == 0x00) {
					ensure = PS_GenChar(CharBuffer2); //��������
					if (ensure == 0x00) {
						OLED_ShowString(1, 1, "    Waiting    ");
						i = 0;
						processnum = 2; //����������
					} else
						ShowErrMessage(ensure);
				} else
					ShowErrMessage(ensure);
				break;

			case 2:
				OLED_ShowString(1, 1, "  AGAIN  ");
				ensure = PS_Match();
				if (ensure == 0x00) {
					OLED_ShowString(1, 1, "    success    ");
					processnum = 3; //�������Ĳ�
				} else {
					OLED_ShowString(1, 1, "    failed    ");
					ShowErrMessage(ensure);
					i = 0;
					processnum = 0; //���ص�һ��
				}
				delay_ms(500);
				break;

			case 3:
				OLED_ShowString(1, 1, "  Creating  ");
				delay_ms(500);
				ensure = PS_RegModel();
				if (ensure == 0x00) {
					OLED_ShowString(1, 1, "Creating Success");
					processnum = 4; //�������岽
				} else {
					processnum = 0;
					ShowErrMessage(ensure);
				}
				delay_ms(1000);
				break;

			case 4:
				OLED_Clear();
				OLED_ShowString(1, 1, " K4add   K2sub");
				OLED_ShowString(2, 1, "    K3 save    ");
				while (key_num != 3) {
					key_num = KEY_Scan(0);
					if (key_num == 2) {
						key_num = 0;
						if (ID_NUM > 0)
							ID_NUM--;
					}
					if (key_num == 4) {
						key_num = 0;
						if (ID_NUM < 99)
							ID_NUM++;
					}
					OLED_ShowString(3, 1, "ID=");
					OLED_ShowNum(3, 4, ID_NUM, 2);
				}
				key_num = 0;
				ensure = PS_StoreChar(CharBuffer2, ID_NUM); //����ģ��
				if (ensure == 0x00) {
					OLED_Clear();
					OLED_ShowString(1, 1, "  saved   ");
					delay_ms(1500);
					OLED_Clear();
					OLED_ShowString(1, 1, "TEST");
					OLED_ShowString(2, 1, "K1 ADD");
					OLED_ShowString(3, 1, "K3 DEL");
					OLED_ShowString(4, 1, "K5 VER");
					return ;
				} else {
					OLED_Clear();
					OLED_ShowString(1, 1, "Retrying");
					processnum = 0;
					ShowErrMessage(ensure);
				}
				break;
		}
		delay_ms(400);
		if (i == 10) { //����5��û�а���ָ���˳�
			OLED_Clear();
			OLED_ShowString(1, 1, "Timeout");
			OLED_ShowString(2, 1, "K1 ADD");
			OLED_ShowString(3, 1, "K3 DEL");
			OLED_ShowString(4, 1, "K5 VER");
			break;
		}
	}
}


//ˢָ��
void press_FR(void) {
	SearchResult seach;
	u8 ensure;
	char str[20];
	while (key_num != 1) {
		key_num = KEY_Scan(0);
		ensure = PS_GetImage();
		if (ensure == 0x00) { //��ȡͼ��ɹ�
			ensure = PS_GenChar(CharBuffer1);
			if (ensure == 0x00) { //���������ɹ�
				ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &seach);
				if (ensure == 0x00) { //�����ɹ�
					OLED_Clear();
					OLED_ShowString(1, 1, "press again");
					OLED_ShowString(2, 1, "K1 back");
					OLED_ShowString(3, 1, "  Succeeded  ");
					sprintf(str, " ID:%d score:%d ", seach.pageID, seach.mathscore);
					OLED_ShowString(4, 1, str);
					delay_ms(1500);
				} else {
					OLED_Clear();
					OLED_ShowString(1, 1, "press again");
					OLED_ShowString(2, 1, "K1 back");
					OLED_ShowString(3, 1, "failed");
					delay_ms(1500);
				}
			}
		}
	}
	OLED_Clear();
	OLED_ShowString(1, 1, "TEST");
	OLED_ShowString(2, 1, "K1 ADD");
	OLED_ShowString(3, 1, "K3 DEL");
	OLED_ShowString(4, 1, "K5 VER");
	delay_ms(500);
}

//ɾ��ָ��
void Del_FR(void) {
	u8  ensure;
	u16 ID_NUM = 0;
	OLED_Clear();
	OLED_ShowString(1, 1, "K4add K2sub");
	OLED_ShowString(2, 1, "K5 del all");
	OLED_ShowString(3, 1, "ID:");
	OLED_ShowString(4, 1, "K1back K3confirm");

	while (key_num != 3) {
		key_num = KEY_Scan(0);
		if (key_num == 2) {
			key_num = 0;
			if (ID_NUM > 0)
				ID_NUM--;
		}
		if (key_num == 4) {
			key_num = 0;
			if (ID_NUM < 99)
				ID_NUM++;
		}
		if (key_num == 1)
			goto MENU ; //������ҳ��
		if (key_num == 5) {
			key_num = 0;
			ensure = PS_Empty(); //���ָ�ƿ�
			if (ensure == 0) {
				OLED_Clear();
				OLED_ShowString(1, 1, "   Succeeded   ");
			} else
				ShowErrMessage(ensure);
			delay_ms(1500);
			goto MENU ; //������ҳ��
		}
		OLED_ShowNum(3, 4, ID_NUM, 2);
	}
	ensure = PS_DeletChar(ID_NUM, 1); //ɾ������ָ��
	if (ensure == 0) {
		OLED_Clear();
		OLED_ShowString(1, 1, "   Succeeded   ");
	} else
		ShowErrMessage(ensure);
	delay_ms(1500);
MENU:
	OLED_Clear();
	OLED_ShowString(1, 1, "TEST");
	OLED_ShowString(2, 1, "K1 ADD");
	OLED_ShowString(3, 1, "K3 DEL");
	OLED_ShowString(4, 1, "K5 VER");
	key_num = 0;
}

