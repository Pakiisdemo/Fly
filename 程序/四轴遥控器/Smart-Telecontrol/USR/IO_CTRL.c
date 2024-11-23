#include "pbdata.h" 
u8 LOCK_flag=0;       //上锁
u8 UNLOCK_flag=0;     //解锁
u8 TRUN_LEFT_flag=0;  //航向左
u8 TURN_RIGHT_flag=0; //航向右
u8 yaw_flag=0;        //航向数据
u8 fine_tuning_flag=0;  //微调权限，1表示只能微调忽略摇杆数据
u8 auto_land_flag=0;    //自动降落，给定固定油门数值
u8 stunt_flag=0;        //特技飞行标志位
u8 auto_oil_temp=0;     //自动降落油门数值缓存
//************按键引脚配置*************************
void KEY_RCC_Configuration(void)
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}
//**************按键引脚配置**************************
void KEY_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
																|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
																|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_15;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//**************按键引脚配置***************************
void KEY_EINT_Configration(void)
{
   EXTI_InitTypeDef  EXTI_InitStructure;	
	   	
	 EXTI_ClearITPendingBit(EXTI_Line3);                      
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	 EXTI_InitStructure.EXTI_Line=EXTI_Line3;                
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;        
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;    
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;                
	 EXTI_Init(&EXTI_InitStructure);  
	
	 EXTI_ClearITPendingBit(EXTI_Line4);                      
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;                
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;        
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;    
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;                
	 EXTI_Init(&EXTI_InitStructure); 
	 	 
	 EXTI_ClearITPendingBit(EXTI_Line9);                      
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	 EXTI_InitStructure.EXTI_Line=EXTI_Line9;                
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;        
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;    
	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;                
	 EXTI_Init(&EXTI_InitStructure);
}
//*****************按键引脚配置************************
void KEY_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;  
 
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             
		NVIC_InitStructure.NVIC_IRQChannel =EXTI3_IRQn;          
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;   
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
		NVIC_Init(&NVIC_InitStructure); 

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             
		NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;          
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
		NVIC_Init(&NVIC_InitStructure); 	

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             
		NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;          
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;   
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
		NVIC_Init(&NVIC_InitStructure); 	
}
//*********************按键初始化****************************
void KEY_Init(void)
{
	KEY_RCC_Configuration();
  KEY_GPIO_Configuration();
  KEY_EINT_Configration();
  KEY_NVIC_Configuration();
}
//**************航向数据获取************************
void yaw_status(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
	 TRUN_LEFT_flag=1;
	}
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)
	{
	 TRUN_LEFT_flag=0;
	}
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
	 TURN_RIGHT_flag=1;
	}
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1)
	{
	 TURN_RIGHT_flag=0;
	}
	
	if(((TRUN_LEFT_flag==0)&&(TURN_RIGHT_flag==0))||((TRUN_LEFT_flag==1)&&(TURN_RIGHT_flag==1)))
	{
	  yaw_flag=0;
	}
	if((TRUN_LEFT_flag==1)&&(TURN_RIGHT_flag==0))
	{
	  yaw_flag=1;
	}	
	if((TRUN_LEFT_flag==0)&&(TURN_RIGHT_flag==1))
	{
	  yaw_flag=2;
	}
}
//****************姿态微调控制***********************
void fine_tuning(void)
{
	//微调标志位控制，防止微调和摇杆数据冲突
	//有微调按键按下
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)||
		 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)||
	   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)||
	   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0))
	{
	  	fine_tuning_flag=1;
	}
	//没有微调按键按下
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)&&
		 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1)&&
	   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1)&&
	   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1))
	{
	  	fine_tuning_flag=0;
	}
	//自动降落控制
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
	{
		if(Oil_n>OIL_AUTO)    //由当前油门数值逐渐减小到可以平稳降落的数值
		{
		  Oil_n--;
			auto_oil_temp=Oil_n;
			delay_ms(OIL_AUTO_SPEED);
		}		
	  auto_land_flag=1;
	}
	//松开自动降落按钮，还需要把油门摇杆拉低到小于OIL_AUTO才可以恢复油门摇杆的控制权
	if(auto_land_flag==1)
	{
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==1)
		{
			Oil_ADC();   //油门状态检测
			if(Oil_n<=OIL_AUTO)
			{
			 auto_land_flag=0;  //恢复摇杆控制权
			}
			else
			{
				Oil_n=auto_oil_temp;  //油门值等于自动降落按钮缓存的数值
			}			
		}
	}		
	//前进微调控制
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)
	{	 
		Pitch_n=60;	
	}
	//后退微调控制
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
	{
		Pitch_n=40;
	}
	//左翻滚微调控制
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
	{
		Roll_n=40;
	}
	//右翻滚微调控制
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Roll_n=60;
	}	
}
//*****************上锁****************************
void EXTI3_IRQHandler(void)             
{	
 	delay_ms(5);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0)
  {
		//安全性考虑油门摇杆必须为0才可以上锁
		Oil_ADC();   //油门状态检测
		if(Oil_n==0)
		{
			msg_send[0]=0x01;
			msg_send[1]=0x01;
			msg_send[2]=0x00;
			msg_send[3]=0x00;
			msg_send[4]=0x00;
			MSG_Send(msg_send);
			lock_flag=0;  //上锁飞机和遥控器
			OLED_ShowString(30,52,"Please unlock      ",8,1);	
		}			
		else
		{
			OLED_ShowString(30,52,"Oil=0 to lock    ",8,1);	
		}   
  }
	EXTI_ClearITPendingBit(EXTI_Line3);	
}
//*****************自定义1****************************
void EXTI4_IRQHandler(void)             
{	
 	delay_ms(5);
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)
  {
		stunt_flag=1;   //特技飞行		
  }
	EXTI_ClearITPendingBit(EXTI_Line4);	
}
//****************************************************
void EXTI9_5_IRQHandler(void)             
{	
 	delay_ms(5);
	//解锁
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==0)
  {
		//安全性考虑油门摇杆必须为0才可以解锁
		Oil_ADC();   //油门状态检测
		if(Oil_n==0)
		{
			msg_send[0]=0x01;
			msg_send[1]=0x02;
			msg_send[2]=0x00;
			msg_send[3]=0x00;
			msg_send[4]=0x00;
			MSG_Send(msg_send);
			lock_flag=1;  //解锁飞机和遥控器
			OLED_ShowString(30,52,"Safe operation     ",8,1);			
		}
		else
		{
			OLED_ShowString(30,52,"Oil=0 to unlock     ",8,1);	
		}		  				
		EXTI_ClearITPendingBit(EXTI_Line9);	
  }
}









