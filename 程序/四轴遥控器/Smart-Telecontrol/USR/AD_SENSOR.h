#ifndef _AD_SENSOR_H
#define _AD_SENSOR_H
#include "pbdata.h"
extern float tmp_num;
extern u8 tmp_value;
extern float Oil_adc;
extern float Pitch_adc;
extern float Roll_adc;
extern float Bat_ad;
extern u8 Oil_n;      //油门百分比
extern u8 Pitch_n;    //俯仰百分比
extern u8 Roll_n;     //翻滚百分比
extern u8 Bat_n;      //手柄电量百分比
void ADC_RCC(void);                     //ADC时钟配置
void ADC_GPIO(void);                    //ADC管脚配置
void ADC_Configuration(void);           //ADC系统配置
void ADC_Check_Init(void);              //ADC初始化函数
u16 T_Get_Adc(u8 ch);                   //获得指定通道的采样值
u16 T_Get_Adc_Average(u8 ch,u8 times);  //获得指定通道的平均采样值
void Pitch_ADC(void);
void Roll_ADC(void);
void Oil_ADC(void);
void BAT_ADC(void);
void Temperature_check(void);
#endif
































