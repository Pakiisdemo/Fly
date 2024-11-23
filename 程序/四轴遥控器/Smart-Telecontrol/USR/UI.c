#include "pbdata.h"
u8 lock_flag=0;          //解锁状态，1表示解锁，0表示上锁
u8 Plane_status=0;       //飞机状态。1表示上锁，2表示解锁
u8 Plane_Bat=0;          //飞机电量，由串口获取
u8 out_control_flag=0;   //失控标志位
u8 Plane_Bat_low_flag=0; //飞机低电量标志位
u8 Remote_Bat_low_flag=0;//手柄低电量标志位
//***************界面显示函数**************************
void photo(u8 n)
{
	//logo页面
  if(n==0)
	{
		OLED_Clear();  
	  OLED_ShowPicture(31,0,64,64,logo,1); //显示logo
    OLED_Refresh();	
	}
	//首页
	if(n==1)
	{
		OLED_Clear();
	  OLED_ShowString(0,2,"Plane-Bat:",8,1);
		OLED_ShowString(0,12,"Remote-Bat:",8,1);
		OLED_ShowString(0,22,"Oil:",8,1);
		OLED_ShowString(0,32,"Pitch:",8,1);
		OLED_ShowString(0,42,"Roll:",8,1);
		OLED_ShowString(0,52,"Tips:",8,1);
		OLED_ShowString(30,52,"Please unlock       ",8,1);			
    OLED_Refresh();	
	}
}
//***********数据显示函数*****************************
// n:解锁标志位
void num_show(u8 n)
{
	BAT_ADC();          //检测手柄电量
	//公共状态，电量显示
	//飞机电量显示
	if(Plane_Bat<10)
	{
	 OLED_ShowNum(80,2,Plane_Bat,1,8,1); 
	 OLED_ShowString(86,2,"    ",8,1);
	}
	if((Plane_Bat>=10)&&(Plane_Bat<100))
	{
	 OLED_ShowNum(80,2,Plane_Bat,2,8,1); 
	 OLED_ShowString(93,2,"   ",8,1);
	}
	if(Plane_Bat>=100)
	{
	 OLED_ShowNum(80,2,Plane_Bat,3,8,1); 
	 OLED_ShowString(102,2,"  ",8,1);
	}
	//手柄电量显示
  if(Bat_n<10)
	{
	 OLED_ShowNum(80,12,Bat_n,1,8,1);
   OLED_ShowString(86,12,"    ",8,1);		
	}
	if((Bat_n>=10)&&(Bat_n<100))
	{
	 OLED_ShowNum(80,12,Bat_n,2,8,1); 
	 OLED_ShowString(93,12,"   ",8,1);
	}
	if(Bat_n>=100)
	{
	 OLED_ShowNum(80,12,Bat_n,3,8,1); 
	 OLED_ShowString(102,12,"  ",8,1);
	}	
	//上锁状态
   if(n==0)
	 {
	  OLED_ShowString(80,22,"lock    ",8,1);
		OLED_ShowString(80,32,"lock    ",8,1);
		OLED_ShowString(80,42,"lock    ",8,1); 
	 }	
	//解锁状态
   if(n==1)
	 {
		//油门数值显示
	  if(Oil_n<10)
		{
		 OLED_ShowNum(80,22,Oil_n,1,8,1); 
		 OLED_ShowString(86,22,"      ",8,1);
		}
		if((Oil_n>=10)&&(Oil_n<100))
		{
		 OLED_ShowNum(80,22,Oil_n,2,8,1); 
		 OLED_ShowString(93,22,"     ",8,1);
		}
		if(Oil_n>=100)
		{
		 OLED_ShowNum(80,22,Oil_n,3,8,1); 
		 OLED_ShowString(102,22,"    ",8,1);
		}
		//俯仰数值显示
	  if(Pitch_n<10)
		{
		 OLED_ShowNum(80,32,Pitch_n,1,8,1); 
		 OLED_ShowString(86,32,"      ",8,1);
		}
		if((Pitch_n>=10)&&(Pitch_n<100))
		{
		 OLED_ShowNum(80,32,Pitch_n,2,8,1); 
		 OLED_ShowString(93,32,"     ",8,1);
		}
		if(Pitch_n>=100)
		{
		 OLED_ShowNum(80,32,Pitch_n,3,8,1);
     OLED_ShowString(102,32,"    ",8,1);			
		}
		//翻滚数值显示
	  if(Roll_n<10)
		{
		 OLED_ShowNum(80,42,Roll_n,1,8,1); 
		 OLED_ShowString(86,42,"      ",8,1);
		}
		if((Roll_n>=10)&&(Roll_n<100))
		{
		 OLED_ShowNum(80,42,Roll_n,2,8,1); 
		 OLED_ShowString(93,42,"     ",8,1);
		}
		if(Roll_n>=100)
		{
		 OLED_ShowNum(80,42,Roll_n,3,8,1); 
		 OLED_ShowString(102,42,"    ",8,1);
		}		
	 }
   OLED_Refresh();	   //刷新数值	 
}
//*****************灯光提示************************
void LED_Show(void)
{
  BAT_ADC();          //检测手柄电量
	if(Plane_Bat<=20)   //飞机低电量
	{
	  Plane_Bat_low_flag=1;
	}
	if(Plane_Bat>20)   //飞机电量正常
	{
	  Plane_Bat_low_flag=0;
		LED(2,0);         //关灯
	}
	if(Bat_n<=20)       //手柄低电量
	{
	  Remote_Bat_low_flag=1;
	}
	if(Bat_n>20)        //手柄电量正常
	{
	  Remote_Bat_low_flag=0;
		LED(3,0);        //关灯
	}
}
//**************飞机控制*************************
void Plane_Control(void)
{
 if(lock_flag==1)   //解锁状态
 {
		if(auto_land_flag==0)
		{
		 Oil_ADC();   //油门状态检测
		}
    if(fine_tuning_flag==0)	 
		{		 
		 Pitch_ADC();  //俯仰状态检测    
		 Roll_ADC();   //翻滚状态检测
		}		
		fine_tuning(); //微调姿态检测
		yaw_status(); 		
		msg_send[0]=0x02;
		msg_send[1]=Oil_n;
		msg_send[2]=Pitch_n;
		msg_send[3]=Roll_n;
		msg_send[4]=yaw_flag;		
		MSG_Send(msg_send);
 }	
}
//*******交互函数************************
void UI(void)
{ 	
  num_show(lock_flag);
	LED_Show();

}



