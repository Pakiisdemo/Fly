#ifndef _remote_H
#define _remote_H
#include "pbdata.h"
extern u8 USART1_MSG_flag;
extern u8 USART1_MSG_Value;
extern u8 USART1_Count;
extern u8 USART1_Cmd_Flag;
extern u8 USART1_Receieve[8];  
extern u8 Data1;
extern u8 Data2;
extern u8 msg_send[5]; 
void USART1_MSG_RCC_Configuration(void);
void USART1_MSG_GPIO_Configuration(void);
void USART1_MSG_Configration(void);
void USART1_MSG_NVIC_Configuration(void);
void USART1_MSG_Init(void);
void USART1_IRQHandler(void);
void Receive_Data(void);
void MSG_Send(u8 msg[5]);
void MSG_Recieve(void);
#endif




