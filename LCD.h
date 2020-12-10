#ifndef __LCD_H
#define __LCD_H	
#include"font.h"	
#include "sys.h" 
/////////////////////////////////////�û�������///////////////////////////////////	 
//֧�ֺ��������ٶ����л���֧��8/16λģʽ�л�
#define USE_HORIZONTAL  	0	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
#define LCD_USE8BIT_MODEL   0 	//�������������Ƿ�ʹ��8λģʽ 0,ʹ��16λģʽ.1,ʹ��8λģʽ

//IO����
#define  LCD_DataPortH P3     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
#define  LCD_DataPortL P2    //��8λ���ݿ�	,8λģʽ�µ�8λ���Բ�����,��ȷ��P0���Ѿ�����10K����,����̫С����С4.7K,�Ƽ�10K.
sbit LCD_RS = P0^5;  		 //����/�����л�
sbit LCD_WR = P0^4;		  //д����
sbit LCD_RD =P0^3;		     //������
sbit LCD_CS=P0^6;		//Ƭѡ	
sbit LCD_RESET = P3^4;	      //��λ 
sbit LCD_BL=P3^5;		//�������

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ����
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����

void delay_ms(int count);
void LCD_Init(void); 

void LCD_WR_DATA8(char VH,char VL); //��������-8λ����
void LCD_WR_DATA(int Data);
void LCD_WR_REG(int Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//���ù��λ��
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//������ʾ����
void LCD_DrawPoint(u16 x,u16 y,u16 color);//����
void LCD_WriteRAM_Prepare(void);
void LCD_SetParam(void);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ 
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ  
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
					  		 
#endif  
	 
	 


