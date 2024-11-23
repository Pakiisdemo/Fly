#include "pbdata.h" 
u16 time2=0;         //基准定时单位毫秒
u16 time3=0;         //基准定时单位毫秒
u16 time_out=0;      //失控判断
u8 LED_Control=0;   //灯光闪烁变量
u8 LED_Plane=0;     //灯光闪烁变量
u8 LED_Remote=0;    //灯光闪烁变量
//**********************配置系统时钟*********************************
void time_RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //打开time2的中断时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //打开time3的中断时钟
}
//**********************时钟中断配置函数*********************************
void time_TIM2_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 	                //清除定时器中断
  TIM_TimeBaseStructure.TIM_Period=3599;                      //计数3600,1毫秒  
  TIM_TimeBaseStructure.TIM_Prescaler=19;                     //20分频
  TIM_TimeBaseStructure.TIM_ClockDivision=0;                  //不滤波
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);              //初始化
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                    //打开定时器中断
	TIM_Cmd(TIM2,ENABLE);                                       //打开定时器外设
}
//**************************配置优先级***********************************
void time2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                        //为结构体定义结构体变量
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             //对优先级进行分组
  NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;              //外部中断5引脚
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级为1         
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能
  NVIC_Init(&NVIC_InitStructure);                             //初始化
}
//**********************时钟中断配置函数*********************************
void time_TIM3_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 	                //清除定时器中断
  TIM_TimeBaseStructure.TIM_Period=3599;                      //计数3600,1毫秒  
  TIM_TimeBaseStructure.TIM_Prescaler=19;                     //20分频
  TIM_TimeBaseStructure.TIM_ClockDivision=0;                  //不滤波
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);              //初始化
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                    //打开定时器中断
	TIM_Cmd(TIM3,ENABLE);                                       //打开定时器外设
}
//**************************配置优先级***********************************
void time3_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                        //为结构体定义结构体变量
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             //对优先级进行分组
  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;              //外部中断5引脚
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //响应优先级为1         
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能
  NVIC_Init(&NVIC_InitStructure);                             //初始化
}

//*********************定时器初始化********************************************
void time_init(void)
{
  time_RCC_Configuration();
	time_TIM2_Configuration();
	time_TIM3_Configuration();
  time2_NVIC_Configuration();
	time3_NVIC_Configuration();
}
//*************************************中断函数配置 ***************************
void TIM2_IRQHandler(void)
{
	time2++;
	dog_time++;   
  
	//定时喂狗
	if(dog_time>=300)
	{
	 dog_time=0;
	 Feed_dog();      //喂狗
	}
	//判断是否失控
	if(lock_flag==1) //解锁状态下
	{
	  if(USART1_Cmd_Flag==0)  //没有飞机数据
		{
		  time_out++;
		}
		if(USART1_Cmd_Flag!=0)  //收到飞机数据
		{
		  time_out=0;   
			out_control_flag=0;  //关闭失控提示
			LED(1,0); 
		}
		if(time_out>=500)
		{
			time_out=0;
		  out_control_flag=1;  //失控了
		}		
	}
	
	if(time2>=200)
	{
	  time2=0;
		//LED指示灯提示
		if(out_control_flag==1)    //失控提示
		{
			LED_Control=~LED_Control;
			LED(1,LED_Control); 
		  OLED_ShowString(30,52,"Out of control! ",8,1);	
		}
		if(Plane_Bat_low_flag==1)  //飞机低电量提示
		{
			LED_Plane=~LED_Plane;
			LED(2,LED_Plane); 
			OLED_ShowString(30,52,"Plane Bat low! ",8,1);
		}
		if(Remote_Bat_low_flag==1) //手柄低电量提示
		{
			LED_Remote=~LED_Remote;
			LED(3,LED_Remote); 
			OLED_ShowString(30,52,"Remote Bat low!",8,1);
		}
	}
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 	         //清除定时器中断	
}
//*************************************中断函数配置***************************
void TIM3_IRQHandler(void)
{
	time3++;
	MSG_Recieve();     //解码	
	
	if(time3>=100)
	{
		time3=0;
    if(stunt_flag==1)  //特技飞行
		{
			stunt_flag=0;
			
		  if(Oil_n>=50)   //油门数值大于50允许特技飞行
			{
				msg_send[0]=0x03;
				msg_send[1]=0x00;
				msg_send[2]=0x00;
				msg_send[3]=0x00;
				msg_send[4]=0x00;
				MSG_Send(msg_send);
			}
		}
    if(stunt_flag==0) //正常飞行
		{
		 Plane_Control();	//发送飞机姿态	
		}					   
	}
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 	         //清除定时器中断	
}





















