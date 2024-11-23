#include "pbdata.h"
u8 test_n=0;
u32 user_num=0;
//**********************不精确微妙延时*****************************
void us(u16 time) 
{ u16 i=0;    
  while(time--)   
 {       
  i=10;     
  while(i--);       
 } 
} 
//***********************不精确毫秒延时****************************
void ms(u16 time) 
{ 
  u16 i=0;    
  while(time--)   
 {      
  i=12000;       
 while(i--);      
 } 
} 
//***********************微秒延时**精确******************************
void delay_us(u32 nus)
{
	 u32 temp;
	 SysTick->LOAD = 9*nus;
	 SysTick->VAL=0X00;                                     //清空计数器
	 SysTick->CTRL=0X01;                                    //使能，减到0是无动作，采用外部时钟源
	 do
	 {
	  temp=SysTick->CTRL;                                  //读取当前倒计数值
	 }while((temp&0x01)&&(!(temp&(1<<16))));               //等待时间到达
	 
	 SysTick->CTRL=0x00;                                   //关闭计数器
	 SysTick->VAL =0X00;                                   //清空计数器
}

//***********************毫秒延时**精确******************************
void delay_ms(u16 nms)
{
	 u32 temp;
	 SysTick->LOAD = 9000*nms;
	 SysTick->VAL=0X00;                                   //清空计数器
	 SysTick->CTRL=0X01;                                  //使能，减到0是无动作，采用外部时钟源
	 do
	 {
	  temp=SysTick->CTRL;                                 //读取当前倒计数值
	 }while((temp&0x01)&&(!(temp&(1<<16))));              //等待时间到达
	 SysTick->CTRL=0x00;                                  //关闭计数器
	 SysTick->VAL =0X00;                                  //清空计数器
}
//******************JTAG引脚复用取消******************************
//说明：
//JTAG复用取消只能调用一次语句，否则死机。并且初始化的GPIO要放在所有GPIO初始化的后面
//建议单独处理
//****************************************************************
void JTAG_IO_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;	
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
				
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;            //V3+
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //模式3
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;            //强度增加
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//***************硬件初始化*************************************
void Hard_Init(void)
{
	SystemInit();                  //初始化STM32	
  JTAG_IO_Init();                //取消引脚复用	
	OLED_Init();                   //屏幕初始化
	OLED_ColorTurn(0);             //颜色反色
  OLED_DisplayTurn(0);           //屏幕翻转
	OLED_Clear();                  //清屏  
	LED_Init();                    //LED初始化
	LED(1,0);                      //默认关灯
	LED(2,0);											 //默认关灯
	LED(3,0);											 //默认关灯
  time_init();	                 //定时器初始化
	USART1_MSG_Init();             //串口1通讯初始化
	KEY_Init();                    //按键初始化
	ADC_Check_Init();              //ADC初始化
	WATCH_DOG_Init();              //看门狗初始化
}




































