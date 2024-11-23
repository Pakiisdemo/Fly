#include "pbdata.h"
float tmp_num=0;
float Oil_adc=0.0f;
float Pitch_adc=0.0f;
float Roll_adc=0.0f;
float Bat_adc=0.0f;
u8 Oil_n=0;      //油门百分比
u8 Pitch_n=0;    //俯仰百分比
u8 Roll_n=0;     //翻滚百分比
u8 Bat_n=0;      //手柄电量百分比
u8 tmp_value=0;      //温度值
//**********************配置系统时钟*********************************
void ADC_RCC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ADC PA1端口时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//打开ADC功能的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   //对ADC专用时钟进行6分频12MHz
}
//**********************配置GPIO管脚*********************************
void ADC_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
//ADC管脚配置 PA1,PA2,PA3,PA4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //模拟量输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}	
//**************************ADC配置函数**********************************
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;                            //定义结构体变量
	ADC_DeInit(ADC1);                                             //将外设ADC1的全部寄存器重设为缺省值
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;            //独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                 //单通道扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;           //单次循环，只采集一次
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//采用软件触发模式
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //采用右对齐模式
  ADC_InitStructure.ADC_NbrOfChannel = 4;                       //只用了4个通道所以赋值为4
  ADC_Init(ADC1, &ADC_InitStructure);                           //ADC初始化
	ADC_TempSensorVrefintCmd(DISABLE);                            //关闭内部温度传感器
	ADC_Cmd(ADC1, ENABLE);	                                      //使能指定的ADC1
	ADC_ResetCalibration(ADC1);	                                  //重置指定的ADC1的复位寄存器
  while(ADC_GetResetCalibrationStatus(ADC1));	                  //获取ADC1重置校准寄存器的状态，设置状态为等待
	ADC_StartCalibration(ADC1);	 
	while(ADC_GetCalibrationStatus(ADC1));		                    //获取指定ADC1的校准程序，设置状态为等待
}
//*********************ADC初始化函数**************************************
void ADC_Check_Init(void)
{
  ADC_RCC(); 
  ADC_GPIO();	
	ADC_Configuration();
}
//*****************获得某个通道的值**************************************
u16 T_Get_Adc(u8 ch)   
{ 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	return ADC_GetConversionValue(ADC1);	
}
//*****************获取times次的平均值**********************************
u16 T_Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=T_Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
//************前后控制摇杆ADC检测****************
void Pitch_ADC(void)
{
 u32 ad=0;
 ad=T_Get_Adc_Average(ADC_Channel_2,10);   //获取AD原始值
 Pitch_adc=(float)ad*(3.3/4096);          //转换为电压值
	if(Pitch_adc<PITCH_MID)                  //转换为百分比 回中值为50
 {
   Pitch_n=(u8)(50/(PITCH_MID-PITCH_MIN)*(Pitch_adc-PITCH_MIN));
 }
 if(Pitch_adc>=PITCH_MID)
 {
   Pitch_n=50+(u8)(50/(PITCH_MAX-PITCH_MID)*(Pitch_adc-PITCH_MID));
 }
 //限制幅度
 if(Pitch_n<=0)
 {
   Pitch_n=0;
 }
 //防止回中误差导致数据无法回中，因此扩大回中的范围,摇杆在49-51之间都认为回中
 if((Pitch_n>=49)&&(Pitch_n<=51))
 {
   Pitch_n=50;
 }
 if(Pitch_n>=100)
 {
   Pitch_n=100;
 }
}
//************左右翻滚控制摇杆ADC检测************
void Roll_ADC(void)
{
 u32 ad=0;
 ad=T_Get_Adc_Average(ADC_Channel_1,10);  //获取AD原始值
 Roll_adc=(float)ad*(3.3/4096);  					//转换为电压值
 if(Roll_adc<ROLL_MID)										//转换为百分比 回中值为50
 {
   Roll_n=(u8)(50/(ROLL_MID-ROLL_MIN)*(Roll_adc-ROLL_MIN));
 }
 if(Roll_adc>=ROLL_MID)
 {
   Roll_n=(u8)50+(50/(ROLL_MAX-ROLL_MID)*(Roll_adc-ROLL_MID));
 }
 //限制幅度
 if(Roll_n<=0)
 {
   Roll_n=0;
 }
 //防止回中误差导致数据无法回中，因此扩大回中的范围,摇杆在49-51之间都认为回中
 if((Roll_n>=49)&&(Roll_n<=51))
 {
   Roll_n=50;
 }
 if(Roll_n>=100)
 {
   Roll_n=100;
 }
}
//************油门控制摇杆ADC检测****************
void Oil_ADC(void)
{
	u32 ad=0;
	ad=T_Get_Adc_Average(ADC_Channel_3,10);   //获取AD原始值
  Oil_adc=(float)ad*(3.3/4096);             //转换为电压值
	Oil_n=(u8)(100/(OIL_MAX-OIL_MIN)*(Oil_adc-OIL_MIN));//转换为百分比
	//限制幅度
	if(Oil_n<=2) //受物理环境影响，有可能无法到0，因此扩大范围
	{
	  Oil_n=0;
	}
	if(Oil_n>=100)
	{
		Oil_n=100;
	}
}
//************电池电量ADC检测****************
void BAT_ADC(void)
{
	u32 ad=0;
	ad=T_Get_Adc_Average(ADC_Channel_4,10);
  Bat_adc=(float)ad*(3.3/4096);  //此值为电池电压分压值，电池电压/188*120
	Bat_adc=Bat_adc*1.5667;        //倒推出电池电压为 检测值/120*188  相当于检测值*（188/120）
	Bat_n=(u8)(100/(BAT_MAX-BAT_MIN)*(Bat_adc-BAT_MIN));//转换为百分比
	//限制幅度
	if(Bat_n<=0)
	{
	 Bat_n=0;
	}
	if(Bat_n>=100)
	{
		Bat_n=100;
	}
}
//*********************温度检测函数**************************************
void Temperature_check(void)
{
  u32 ad=0;                    //初始化ad变量
	ad=T_Get_Adc_Average(ADC_Channel_16,10);  //通道16为温度采样通道，取10次平均值	 	
  tmp_num=(float)ad*(3.3/4096);
	//25℃对应 1430mV 
	//变化1℃ 对应电压变化4.3mV 
	tmp_value=(u8)(((1.43-tmp_num)/0.0043+25)/1.32);     //实际温度比测量温度低，所以添加修正系数1.32
	
}



































