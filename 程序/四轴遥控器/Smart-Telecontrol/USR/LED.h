#ifndef _LED_H
#define _LED_H
#include "pbdata.h"
void LED_RCC_Configuration(void);
void LED_GPIO_Configuration(void);
void LED_Init(void);
void LED(u8 type,u8 n);
#endif


