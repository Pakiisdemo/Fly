#ifndef _time_H
#define _time_H
#include "pbdata.h"
//******************定义变量***************************************
extern u16 time2;
extern u16 time3;
extern u16 time_out;
extern u8 LED_Control;   //灯光闪烁变量
extern u8 LED_Plane;     //灯光闪烁变量
extern u8 LED_Remote;    //灯光闪烁变量
//**********************申明函数************************************
void time_RCC_Configuration(void);                //定时器时钟初始化
void time_TIM2_Configuration(void);               //定时器中断函数配置
void time2_NVIC_Configuration(void);              //定时器优先级配置
void time_TIM3_Configuration(void);               //定时器中断函数配置
void time3_NVIC_Configuration(void);              //定时器优先级配置
void time_init(void);                             //定时器初始化
void TIM2_IRQHandler(void);                       //定时器中断代码
void TIM3_IRQHandler(void);                       //定时器中断代码
#endif



