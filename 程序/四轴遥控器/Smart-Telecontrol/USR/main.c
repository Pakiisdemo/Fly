#include "pbdata.h"
int main(void)
{ 
	Hard_Init();     //系统初始化
	photo(0);        //显示LOGO页面
  delay_ms(1000); 
  photo(1);        //显示首页
  while(1)
  {
    UI(); 
  }  
}

