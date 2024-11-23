#ifndef _UI_H
#define _UI_H
#include "pbdata.h"

extern u8 lock_flag;
extern u8 Plane_Bat;
extern u8 Plane_status;
extern u8 out_control_flag;   //失控标志位
extern u8 Plane_Bat_low_flag; //飞机低电量标志位
extern u8 Remote_Bat_low_flag;//手柄低电量标志位

void photo(u8 n);
void num_show(u8 n);
void LED_Show(void);
void Plane_Control(void);
void UI(void);
#endif


