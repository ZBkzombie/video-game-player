#include<reg51.h>
#include "LCD.h"
#include "sys.h"
#include "FC.h"
#include "lcd.h"
 
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=YELLOW;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

void delay_ms(int count)  // /* X1ms */
{
    u8 i=0;
    while(count--)
	  while(i)
	     i++;
}
//******************************************************************
//��������  LCD_Write_Bus
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д������(LCD�����ڲ�����)
//���������VH:��8λ����
//        	VL:��8λ����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
void LCD_Write_Bus(char VH,char VL)  
{	
	LCD_CS=0;
	LCD_DataPortH=VH;	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=VL;		
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}
#else	//16λ��������д��ʱ�� 	
void LCD_Write_Bus(char VH,char VL)   
{
	LCD_CS=0;	
	LCD_DataPortH=VH;	
	LCD_DataPortL=VL;		
	LCD_WR=0;
	LCD_WR=1; 
	LCD_CS=1;
}
#endif

//
////******************************************************************
////��������  LCD_WR_DATA8
////���ߣ�    xiao��@ȫ������
////���ڣ�    2013-02-22
////���ܣ�    ��Һ��������д��д8λ����
////���������VH:��8λ����
////        	VL:��8λ����
////����ֵ��  ��
////�޸ļ�¼����
////******************************************************************
//void LCD_WR_DATA8(char VH,char VL) 
//{
//	LCD_RS=1;
//	LCD_Write_Bus(VH,VL);
//} 

//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(int Reg)	 
{	
	LCD_RS=0;
	LCD_Write_Bus(Reg>>8,Reg);
} 

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
 void LCD_WR_DATA(int Data)
{
	LCD_RS=1;
	LCD_Write_Bus(Data>>8,Data);
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
} 	 

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Reset(void)
{
	LCD_RESET=1;
	delay_ms(50);	
	LCD_RESET=0;
	delay_ms(50);
	LCD_RESET=1;
	delay_ms(50);
}
//******************************************************************
//��������  LCD_DrawPoint 
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color); 	    
}
//******************************************************************
//��������  LCD_DrawLine 
//���ܣ�    GUI����
//���������x1,y1:�������
//        	x2,y2:�յ����� 
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
//******************************************************************
//��������  LCD_DrawRectangle
//���ܣ�    GUI������(�����)
//���������(x1,y1),(x2,y2):���εĶԽ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
	u16 i,j;			
	u16 width=ex-sx+1; 		//�õ����Ŀ��
	u16 height=ey-sy+1;		//�߶�
	LCD_SetWindows(sx,sy,ex-1,ey-1);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		LCD_WR_DATA(color);	//д������ 	 
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��
}

//******************************************************************
//��������  LCD_ShowChar
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʾ����Ӣ���ַ�
//���������(x,y):�ַ���ʾλ����ʼ����
//        	fc:ǰ�û�����ɫ
//			bc:������ɫ
//			num:��ֵ��0-94��
//			size:�����С
//			mode:ģʽ  0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;      
		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
	if(!mode) //���ģʽ
	{
		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_WR_DATA(fc); 
				else LCD_WR_DATA(bc); 
				temp>>=1;				
		    }			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos,fc);//��һ����    
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��    	   	 	  
} 
//******************************************************************
//��������  LCD_ShowString
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʾӢ���ַ���
//���������x,y :�������	 
//			size:�����С
//			*p:�ַ�����ʼ��ַ
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  	  
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
}
//******************************************************************
//��������  LCD_ShowNum	 
//���ܣ�    ��ʾ�������ֱ���ֵ
//���������x,y :�������	 
//			len :ָ����ʾ���ֵ�λ��
//			size:�����С(12,16)
//			color:��ɫ
//			num:��ֵ(0~4294967295)
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}  			 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,1); 
	}
} 

//���ܣ�    ��ʾһ��16λBMPͼ��
//���������x,y :�������
// 			*p :ͼ��������ʼ��ַ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void Drawbmp16(u16 x,u16 y,const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+40-1,y+40-1);//��������
    for(i=0;i<40*40;i++)
	{	
	 	picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);				
		LCD_WR_DATA(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���ʾ����Ϊȫ��
}
//******************************************************************
//��������  LCD_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{
	LCD_Reset(); //��ʼ��֮ǰ��λ
	LCD_RD=1;	 //RD����û���õ������ߴ���

	//=========================Һ������ʼ��=============================//
	LCD_WriteReg(0X14,0X0030); 
	LCD_WriteReg(0X0F,0X0008); 
	LCD_WriteReg(0X11,0X0231); 
	LCD_WriteReg(0X12,0X0009); 
	LCD_WriteReg(0X13,0X2958); 
	LCD_WriteReg(0X10,0X5268);
	delay_ms(40);
	LCD_WriteReg(0X12,0X0019);
	delay_ms(40);
	LCD_WriteReg(0X10,0X3260); 
	LCD_WriteReg(0X13,0X1958); 
	delay_ms(40);
	//Gamma Setting
	LCD_WriteReg(0X30,0X0000);
	LCD_WriteReg(0X31,0X0501);
	LCD_WriteReg(0X32,0X0207);
	LCD_WriteReg(0X33,0X0502);
	LCD_WriteReg(0X34,0X0007);
	LCD_WriteReg(0X35,0X0601);
	LCD_WriteReg(0X36,0X0707);
	LCD_WriteReg(0X37,0X0305);
	LCD_WriteReg(0X38,0X040e);
	LCD_WriteReg(0X39,0X040e);
	LCD_WriteReg(0X3a,0X0101);
	LCD_WriteReg(0X3b,0X0101);
	LCD_WriteReg(0X3c,0X0101);
	LCD_WriteReg(0X3d,0X0101);
	LCD_WriteReg(0X3e,0X0001);
	LCD_WriteReg(0X3f,0X0001);
	
	LCD_WriteReg(0X21,0x0000);
	
	//Display Mode
	LCD_WriteReg(0X01,0X011B);//0x011b
	LCD_WriteReg(0X02,0X0700);
	LCD_WriteReg(0X03,0X1030);//1030 
	LCD_WriteReg(0X08,0X1010);
	LCD_WriteReg(0X0A,0X4420);
	LCD_WriteReg(0X0B,0X5030);
	LCD_WriteReg(0X0F,0X000D);
	delay_ms(10);
	//Display ON sequence
	LCD_WriteReg(0X07,0X0005);
	LCD_WriteReg(0X07,0X0025);
	LCD_WriteReg(0X07,0X0027);
	LCD_WriteReg(0X07,0X0037);
	//����LCD���Բ���
	LCD_SetParam();//����LCD����	 
	LCD_BL=1;//��������	 
}
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA((xEnd<<8)+xStar);
	
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA((yEnd<<8)+yStar);
	
	LCD_WR_REG(0x0021);
#if USE_HORIZONTAL==1	//ʹ�ú���
	LCD_WR_DATA((xStar<<8)+yStar);
#else
	LCD_WR_DATA((yStar<<8)+xStar);
#endif 	

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   
//
///*************************************************
//��������LCD_SetCursor
//���ܣ����ù��λ��
//��ڲ�����xy����
//����ֵ����
//*************************************************/
//void LCD_SetCursor(u16 Xpos, u16 Ypos)
//{
//	LCD_WR_REG(lcddev.setxcmd);
//	LCD_WR_DATA((175<<8)+Xpos);
//	
//	LCD_WR_REG(lcddev.setycmd);
//	LCD_WR_DATA((219<<8)+Ypos);
//	
//	LCD_WR_REG(0x0021);
//#if USE_HORIZONTAL==1	//ʹ�ú���
//	LCD_WR_DATA((Xpos<<8)+Ypos);
//#else
//	LCD_WR_DATA((Ypos<<8)+Xpos);
//#endif 
//	
//	LCD_WriteRAM_Prepare();	//��ʼд��GRAM 
//
//} 

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x0022;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=220;
	lcddev.height=176;
	lcddev.setxcmd=0x0045;
	lcddev.setycmd=0x0044;			
	LCD_WriteReg(0X03,0X1038);//0x011b
	LCD_WriteReg(0X01,0X031B);//0x011b
#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=176;
	lcddev.height=220;
	lcddev.setxcmd=0x0044;
	lcddev.setycmd=0x0045;	
	LCD_WriteReg(0X03,0X1030);//0x011b
	LCD_WriteReg(0X01,0X011B);//0x011b
#endif
}








//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//
//***********************************************��Ϸ���*******************************//







#define BLOCK_W 10
#define BLOCK_H 10
#define MASK 1
sbit Clock_1=P1^0;
sbit Latch_1=P1^1;
sbit Data_1=P1^2;//�ֱ�һ���Ŷ���
u16 code BackGround[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8191};
u8 code form[][4][4]={//[��״][����][ÿһ��]
	{{0,0,4 ,14},{0,8 ,12,8},{0,0, 14,4},{0,4 ,12,4}},//Ʒ
	{{0 ,0,0,15},{8 ,8 ,8,8},{0 ,0,0,15},{8 ,8 ,8,8}},//һ
	{{0,8 ,12,4},{0,0,6 ,12},{0,8 ,12,4},{0,8 ,12,4}},//
	{{0,0,12,12},{0,0,12,12},{0,0,12,12},{0,0,12,12}},//��
	{{0,0,15,8 },{12,4 ,4,4},{0,0,1 ,15},{8 ,8 ,8,12}}
	};
sbit Clock_2=P1^3;
sbit Latch_2=P1^4;
sbit Data_2=P1^5;//�ֱ������Ŷ���
u8 x;
u8 y;//С�������  ���������ص�����
u8 form_x,form_y; //��״������
u8 number,dir;//

void DrawBlock(u8 dia_x,u8 dia_y){ //��һ��С���飬���Ͻ�Ϊ�������
	u16 x = dia_x*BLOCK_W+1;
	u16 y = dia_y*BLOCK_H+1;
	LCD_DrawRectangle(x,y,x+10,y+10,BLACK);
	LCD_Fill(x+1,y+1,x+10,y+10,GREEN);

} 

void CleanBlock(u8 dia_x,u8 dia_y){//���һ��С����

	u16 x = dia_x*BLOCK_W+1;
	u16 y = dia_y*BLOCK_H+1;
	LCD_DrawRectangle(x,y,x+10,y+10,WHITE);
	LCD_Fill(x+1,y+1,x+10,y+10,WHITE);
}

void CleanForm(u8 form_x,u8 form_y){//����һ����״���������form_x,form_y
	u16 x = form_x*BLOCK_W+1;
	u16 y = form_y*BLOCK_H+1;
	LCD_Fill(x,y,x+41,y+41,WHITE);
}

void delay(){

	u16 delay = 5000;
	while(delay--);
//	delay = 5000;
//	while(delay--);

}
void DrawForm(u8 form_num,u8 dir,u8 form_x,u8 form_y){//form_x,form_yΪ���ϽǵĿ�����,//num�����࣬dir����
	u8 i,j;
	for(j=0;j<4;j++){
		for(i=0;i<4;i++){
			if(form[form_num][dir][j] & (MASK << (3-i))){
				DrawBlock(form_x+i,form_y+j);	
			}
		}
	}	


}
bit Check(void){ //��ͻ���

	if(((form[number][dir][3]<<(13-form_x)) | BackGround[form_y+4]) == ((form[number][dir][3]<<(13-form_x))+BackGround[form_y+4]))
		return 1;
	else 
		return 0;	


}
void Startup(void){//��ʼ��
	LCD_Init();
	LCD_Fill(0,0,176,220,WHITE);
	LCD_DrawRectangle(0,0,133,219,BLACK);
	LCD_DrawRectangle(0,0,132,219,BLACK);
//	BackGround[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8191};
	x=0;
	y=0;
	form_x = 5;
	form_y = 0;
	dir = 0;
	number = 0;
}


void upDateWithInput(void){	 //�������
	char i;	          
   	u8 temp = Read_Key_1();
		for(i = 0;i<8;i++){
			if(temp & (0x01<<i)){
				switch(i){
					case 4: 
						if(dir <3)
							dir++;
						else 
							dir = 0;
						break;
					//case 5:
					//	if(form_y<19) 
					//		form_y++;
					//	break;
					case 6:
						if(form_x>0) 
							form_x--;
						break;
					case 7: 
						if(form_x<9)
							form_x++;
						break;
				 }
			 }
		}



}
void upDateWithoutInput(void){

	 if(Check()){
	 	form_y++;
		CleanForm(form_x,form_y-1);
	 	}
	 



}


void Show(void){

	 DrawForm(number,dir,form_x,form_y);
	 delay();
	 delay();
	 //CleanForm(form_x,form_y);

}
int main (void){
	Startup();
	while(1){
		upDateWithInput();

		upDateWithoutInput();

		Show();			 
		

	
	
	
	}
	return 0;
}