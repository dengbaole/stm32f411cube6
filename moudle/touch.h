#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"

#define TP_PRES_DOWN 0x80  //����������	  
#define TP_CATH_PRES 0x40  //�а��������� 	  

typedef struct {
	u8 (*init)(void);			//��ʼ��������������
	u8 (*scan)(u8);				//ɨ�败����.0,��Ļɨ��;1,��������;
	void (*adjust)(void);		//������У׼
	u16 x0;						//ԭʼ����(��һ�ΰ���ʱ������)
	u16 y0;
	u16 x; 						//��ǰ����(�˴�ɨ��ʱ,����������)
	u16 y;
	u8  sta;					//�ʵ�״̬
	float xfac;
	float yfac;
	short xoff;
	short yoff;
	u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��


#define PEN  		 BIT_ADDR(GPIOB_IDR_Addr,1)   	//T_PEN
#define DOUT 		  BIT_ADDR(GPIOB_IDR_Addr,2)   	//T_MISO
#define TDIN 		PFout(11)  	//T_MOSI
#define TCLK 		PBout(0)  	//T_SCK
#define TCS  		PCout(5)  	//T_CS  

void TP_Write_Byte(u8 num);						//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16* x, u16* y);					//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16* x, u16* y);					//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color); //��һ������У׼��
void TP_Draw_Big_Point(u16 x, u16 y, u16 color);	//��һ�����
u8 TP_Scan(u8 tp);								//ɨ��
void TP_Save_Adjdata(void);						//����У׼����
u8 TP_Get_Adjdata(void);						//��ȡУ׼����
void TP_Adjust(void);							//������У׼
u8 TP_Init(void);								//��ʼ��

void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac); //��ʾУ׼��Ϣ

#endif