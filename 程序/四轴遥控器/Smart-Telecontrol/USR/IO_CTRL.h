#ifndef _IO_CTRL_H
#define _IO_CTRL_H
#include "pbdata.h"

extern u8 LOCK_flag;       //上锁
extern u8 UNLOCK_flag;     //解锁 
extern u8 TRUN_LEFT_flag;  //航向左
extern u8 TURN_RIGHT_flag; //航向右
extern u8 yaw_flag;        //航向数据
extern u8 fine_tuning_flag;
extern u8 auto_land_flag;
extern u8 stunt_flag;
extern u8 auto_oil_temp;

void KEY_RCC_Configuration(void);
void KEY_GPIO_Configuration(void);
void KEY_EINT_Configration(void);
void KEY_NVIC_Configuration(void);
void KEY_Init(void);
void yaw_status(void);    
void fine_tuning(void);
void EXTI3_IRQHandler(void);   
void EXTI4_IRQHandler(void); 
void EXTI9_5_IRQHandler(void);
#endif






