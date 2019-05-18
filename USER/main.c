#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "adc.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	

#include "24cxx.h" 
#include "myiic.h"
#include "touch.h"
u8 Get_touchValue(void);
void para_modify(void);
void result_show(void);
void para_change(void);
u16 touch_x1 = 0xffff,touch_x2 = 0xffff,touch_y1 = 0xffff,touch_y2 = 0xffff;
u16 P_max = 80,P_min = 13,V_max = 95,V_min = 21;
int main(void)
 { 
    u16 adc01,adc02;
	u16 adc01_backup = 0,adc02_backup = 0; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2 
	delay_init();	    			//延时函数初始化	  
	uart_init(9600);	 			//串口初始化为9600			 
	LCD_Init();						//初始化液晶 
	LED_Init();         			//LED初始化	 													    
	KEY_Init();						//按键初始化	
    
	tp_dev.init();					//触摸屏初始化
  
	usmart_dev.init(72);			//usmart初始化	
 	mem_init();						//初始化内存池	    
 	exfuns_init();					//为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1); 			//挂载FLASH.
	while(font_init()) 				//检查字库
	{} 
    LCD_Clear(WHITE);					//清屏	
    POINT_COLOR=RED;
    Show_Str(200,400,200,12,"可以正常显示",12,0);  
		
    para_modify();	
	para_change();
        	
    result_show();
    Adc_Init();		  		        //ADC初始化	
    //POINT_COLOR = BLUE;
    while(1){
        adc01 = Get_Adc_Average(ADC_Channel_1,2);
        adc02 = Get_Adc_Average(ADC_Channel_2,2);	
        LCD_ShowxNum(550,150,adc01,4,24,0);	//监测的结果值
        LCD_ShowxNum(705,150,adc02,4,24,0);
       
        LCD_DrawLine(30+adc01_backup/10, 450 - adc02_backup/10,30 + adc01/10, 450 - adc02/10);
        
        adc01_backup = adc01;
        adc02_backup = adc02;       
    }
}
u8 Get_touchValue(){
	u8 result = 0xff;
	tp_dev.scan(0);	
	touch_x1 = touch_x2;
	touch_y1 = touch_y2;
	touch_x2 = tp_dev.x[0];
	touch_y2 = tp_dev.y[0];
	
	if((touch_x1 == 0xffff)&&(touch_y1 == 0xffff)&&(touch_x2 != 0xffff)&&(touch_y2 != 0xffff)){
		LED0 = 0;
        if((500 < touch_x2)&&(touch_x2 < 600)&&(355 < touch_y2)&&(touch_y2 < 470)){
            result = 0x10;
        }else if((30 < touch_x2)&&(touch_x2 < 230)&&(50 < touch_y2)&&(touch_y2 < 252)){
            result = 0xF1;
        }else if((230 < touch_x2)&&(touch_x2 < 430)&&(50 < touch_y2)&&(touch_y2 < 252)){
            result = 0xF2;
        }else if((30 < touch_x2)&&(touch_x2 < 230)&&(252 < touch_y2)&&(touch_y2 < 450)){
            result = 0xF3;
        }else if((230 < touch_x2)&&(touch_x2 < 430)&&(252 < touch_y2)&&(touch_y2 < 450)){
            result = 0xF4;
        }else if((600 < touch_x2)&&(touch_x2 < 700)&&(355 < touch_y2)&&(touch_y2 < 470)){
            result = 0x00;
        }else if((500 < touch_x2)&&(touch_x2 < 600)&&(10 < touch_y2)&&(touch_y2 < 125)){
            result = 0x01;
        }else if((600 < touch_x2)&&(touch_x2 < 700)&&(10 < touch_y2)&&(touch_y2 < 125)){
            result = 0x02;
        }else if((700 < touch_x2)&&(touch_x2 < 800)&&(10 < touch_y2)&&(touch_y2 < 125)){
            result = 0x03;
        }else if((500 < touch_x2)&&(touch_x2 < 600)&&(125 < touch_y2)&&(touch_y2 < 240)){
            result = 0x04;
        }else if((600 < touch_x2)&&(touch_x2 < 700)&&(125 < touch_y2)&&(touch_y2 < 240)){
            result = 0x05;
        }else if((700 < touch_x2)&&(touch_x2 < 800)&&(125 < touch_y2)&&(touch_y2 < 240)){
            result = 0x06;
        }else if((500 < touch_x2)&&(touch_x2 < 600)&&(240 < touch_y2)&&(touch_y2 < 355)){
            result = 0x07;
        }else if((600 < touch_x2)&&(touch_x2 < 700)&&(240 < touch_y2)&&(touch_y2 < 355)){
            result = 0x08;
        }else if((700 < touch_x2)&&(touch_x2 < 800)&&(240 < touch_y2)&&(touch_y2 < 355)){
            result = 0x09;
        }else if((700 < touch_x2)&&(touch_x2 < 800)&&(355 < touch_y2)&&(touch_y2 < 470)){
            result = 0x11;          
        }		                    
	}else{
		LED0 = 1;
		result = 0xff;
	}	
	return result;
}
void para_modify(void){
	u16 gap;
	u16 x1,x2,x3,x4,y1,y2,y3,y4,y5;
	//width = 480;
	//height = 800;
	gap = 5;
	x1 = 500;
	x2 = 590;
	x3 = 680;
	x4 = 770;	
    
	y1 = 10;
	y2 = 125;
	y3 = 240;
	y4 = 355;
    y5 = 470;
	POINT_COLOR = BLACK;
    
	LCD_DrawLine(x1-5, y1-5, x1-5, y5+5);				//左端的线	
    LCD_DrawLine(x4+5, y1-5, x4+5, y5+5);				//右端的线     
    LCD_DrawLine(x1-5, y1-5, x4+5, y1-5);				//上端的线	
    LCD_DrawLine(x1-5, y5+5, x4+5, y5+5);				//下端的线
	LCD_DrawRectangle(x1+gap, y1+gap, x2-gap, y2-gap);	//1
	LCD_DrawRectangle(x1+gap, y2+gap, x2-gap, y3-gap);	//4
	LCD_DrawRectangle(x1+gap, y3+gap, x2-gap, y4-gap);	//7	
    LCD_DrawRectangle(x1+gap, y4+gap, x2-gap, y5-gap);	//delete       
	LCD_DrawRectangle(x2+gap, y1+gap, x3-gap, y2-gap);	//2
	LCD_DrawRectangle(x2+gap, y2+gap, x3-gap, y3-gap);  //5
	LCD_DrawRectangle(x2+gap, y3+gap, x3-gap, y4-gap);	//8	
    LCD_DrawRectangle(x2+gap, y4+gap, x3-gap, y5-gap);	//0	     
	LCD_DrawRectangle(x3+gap, y1+gap, x4-gap, y2-gap);  //3
	LCD_DrawRectangle(x3+gap, y2+gap, x4-gap, y3-gap);	//6
	LCD_DrawRectangle(x3+gap, y3+gap, x4-gap, y4-gap);	//9	
    LCD_DrawRectangle(x3+gap, y4+gap, x4-gap, y5-gap);	//enter
    
	
	LCD_ShowNum((x1+x2)/2-6,(y1+y2)/2-12,1,1,24);	//1
	LCD_ShowNum((x1+x2)/2-6,(y2+y3)/2-12,4,1,24);	//4
	LCD_ShowNum((x1+x2)/2-6,(y3+y4)/2-12,7,1,24);	//7    
	LCD_ShowNum((x2+x3)/2-6,(y1+y2)/2-12,2,1,24);	//2
	LCD_ShowNum((x2+x3)/2-6,(y2+y3)/2-12,5,1,24);	//5
	LCD_ShowNum((x2+x3)/2-6,(y3+y4)/2-12,8,1,24);	//8
    LCD_ShowNum((x2+x3)/2-6,(y4+y5)/2-12,0,1,24);	//0   
	LCD_ShowNum((x3+x4)/2-6,(y1+y2)/2-12,3,1,24);	//3
	LCD_ShowNum((x3+x4)/2-6,(y2+y3)/2-12,6,1,24);	//6
	LCD_ShowNum((x3+x4)/2-6,(y3+y4)/2-12,9,1,24);	//9   
    LCD_Fill(x1+gap+1,y4+gap+1,x2-gap-1,y5-gap-1,0XF800);
	LCD_ShowString(x1+6,y4+15,60,16,16,"Delete");    
    LCD_Fill(x3+gap+1,y4+gap+1,x4-gap-1,y5-gap-1,0x841F);
	LCD_ShowString(x3+10,y4+15,60,16,16,"Enter");
	
    
    
	LCD_DrawLine(10, 15, 480, 15);
	LCD_DrawLine(10, 60, 480, 60);
	POINT_COLOR = BLUE;
	Show_Str(130,27,300,24,"请输入限制参数P/V",24,0);
	
	Show_Str(50,190,400,24,"P最小值:",24,0);
	Show_Str(250,190,400,24,"P最大值:",24,0);
	Show_Str(50,290,400,24,"V最小值:",24,0);
	Show_Str(250,290,400,24,"V最大值:",24,0);
    
    //LCD_ShowxNum(150,190,1000,4,24,0);
    //LCD_ShowxNum(350,190,1000,4,24,0);
    //LCD_ShowxNum(150,290,1000,4,24,0);
    //LCD_ShowxNum(350,290,1000,4,24,0);
	
	//LCD_DrawRectangle(30, 170, 230, 234);	//enter	
	//LCD_DrawRectangle(230, 170, 430, 234);	//enter	
	
	//LCD_DrawRectangle(30, 270, 230, 334);	//enter	
	//LCD_DrawRectangle(230, 270, 430, 334);	//enter		
    //LCD_DrawRectangle(150, 250, 300, 400);
}
void result_show(void){
	u16 x_x,x_y,y_x,y_y;
	u8 i = 0;
	y_x = 29;
	y_y = 20;
	x_x = 455;
	x_y = 451;
	
	
	LCD_Clear(WHITE); 	
	LCD_DrawLine(475, 15, 800, 15);
	LCD_DrawLine(475, 60, 800, 60);
    LCD_DrawLine(470, 0, 470, 480);
	POINT_COLOR = BLUE;
	Show_Str(500,27,300,24,"监测P/V结果坐标展示",24,0);	
	POINT_COLOR = BLACK;
	LCD_DrawLine(y_x, y_y, y_x, x_y);
	LCD_DrawLine(y_x, x_y, x_x, x_y);
	LCD_DrawLine(y_x, y_y, y_x-4, y_y+6);			//画箭头1_1
	LCD_DrawLine(y_x, y_y, y_x+4, y_y+6);			//画箭头1_1
	LCD_DrawLine(x_x, x_y, x_x-6, x_y-4);			//画箭头1_2
	LCD_DrawLine(x_x, x_y, x_x-6, x_y+4);			//画箭头1_2
	LCD_ShowNum(y_x-5,x_y+2,0,1,16);  				//坐标原点 0 
	
	LCD_ShowString(y_x-23,y_y,20,16,16,"P/");
	LCD_ShowString(x_x-5,x_y+5,32,16,16,"V/");
	for(i=0;i<10;i++){
		LCD_DrawLine(y_x+i*40+39, x_y, y_x+i*40+39, x_y-2);	
	}
	for(i=0;i<10;i++){
		LCD_DrawLine(y_x, x_y-i*38-37, y_x+2, x_y-i*38-37);	
	}
    
    LCD_ShowString(475 ,250 ,200 , 24 ,24 ,"P_min:");
    
	Show_Str(475,150,400,24,"P测值:",24,0);
	Show_Str(630,150,400,24,"V测值:",24,0);
	Show_Str(475,250,400,24,"P_min:",24,0);
	Show_Str(630,250,400,24,"P_max:",24,0);
	Show_Str(475,350,400,24,"V_min:",24,0);
	Show_Str(630,350,400,24,"V_max:",24,0);	
	
	//LCD_ShowxNum(550,150,2000,4,24,0);	//监测的结果值
	//LCD_ShowxNum(705,150,2000,4,24,0);
	LCD_ShowxNum(550,250,P_min,4,24,0);	//P值
	LCD_ShowxNum(705,250,P_max,4,24,0);
	LCD_ShowxNum(550,350,V_min,4,24,0);	//V值
	LCD_ShowxNum(705,350,V_max,4,24,0);
	POINT_COLOR = RED;
	LCD_DrawRectangle(150, 250, 300, 400);
	//POINT_COLOR = BLUE;
	//LCD_DrawLine(30, 450, 400, 100);
	
}
void para_change(void){
    u16 flag = 1;
    u16 temp;    
    u16 value;
    temp = P_min;
    LCD_ShowxNum(200,350,temp,6,24,0); 
    while(1)
	{	       
        value = Get_touchValue();
        switch(value){
            case 0xF1 : flag = 1; temp = P_min; break;
            case 0xF2 : flag = 2; temp = P_max; break;
            case 0xF3 : flag = 3; temp = V_min; break;
            case 0xF4 : flag = 4; temp = V_max; break;
            case 0x11 : flag = 5; break; 
            case 0x00 : temp = temp*10+0; break;            
            case 0x01 : temp = temp*10+1; break; 
            case 0x02 : temp = temp*10+2; break; 
            case 0x03 : temp = temp*10+3; break; 
            case 0x04 : temp = temp*10+4; break; 
            case 0x05 : temp = temp*10+5; break; 
            case 0x06 : temp = temp*10+6; break; 
            case 0x07 : temp = temp*10+7; break; 
            case 0x08 : temp = temp*10+8; break;
            case 0x09 : temp = temp*10+9; break;
            case 0x10 : temp = temp/10; break;
        }       
        LCD_ShowxNum(200,350,temp,6,24,0); 
        LCD_ShowxNum(300,350,value,6,24,0);  
        switch(flag){
            case 1 : P_min = temp; break;
            case 2 : P_max = temp; break;
            case 3 : V_min = temp; break;
            case 4 : V_max = temp; break;           
        } 
               
        LCD_ShowxNum(150,190,P_min,4,24,0);
        LCD_ShowxNum(350,190,P_max,4,24,0);
        LCD_ShowxNum(150,290,V_min,4,24,0);
        LCD_ShowxNum(350,290,V_max,4,24,0);        
        if((P_min > P_max)||(V_min > V_max)){
            Show_Str(100,450,250,24,"请检查参数是否合适!!!",24,0);       
        }else{           
            Show_Str(100,450,250,24,"                      ",24,0); 
        }
        if(flag == 5)
            break;            
     }    

}
    
 

 	
