#ifndef _pbdata_H
#define _pbdata_H
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_usart.h"
#include "misc.h"	 
#include "stm32f10x_tim.h"
#include "math.h"
#include "stdio.h"
#include "time.h"
#include "WATCH_DOG.h"
#include "OLED.h"
#include "UI.h"
#include "IO_CTRL.h"
#include "remote.h"
#include "AD_SENSOR.h"
#include "LED.h"
//********受到电阻焊接精度等影响，摇杆的AD值需要根据仿真调整***********
#define OIL_MAX     3.2895
#define OIL_MIN     0
#define PITCH_MAX   3.2975
#define PITCH_MID   1.6427
#define PITCH_MIN   0
#define ROLL_MAX    3.2975
#define ROLL_MID    1.5912
#define ROLL_MIN    0
//**********电池电量粗略估计，手柄含升压电路，2.8V默认为0,4.1默认为100*****
#define BAT_MAX     4.15
#define BAT_MIN     2.8
//************飞机平稳降落时的油门数值*************************
#define OIL_AUTO 30
//*************飞机自动降落时的降落速度***********************
#define OIL_AUTO_SPEED 150
//************************************************************
extern u8 test_n;
extern u32 user_num;
void us(u16 time);   
void ms(u16 time);
void delay_us(u32 nus);           //微秒函数
void delay_ms(u16 nms); 	         //毫秒函数
void JTAG_IO_Init(void);
void Hard_Init(void);   
#endif



