/***********************************************
公司：轮趣科技(东莞)有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2022-10-13

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2022-10-13

All rights reserved
***********************************************/
#include "control.h"	
#include "Emm_V5.h"
#include "math.h"
extern int Usart_Compelet; //串口接收完一组数据标志
extern uint8_t Urxbuf[9];
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/

/**************************************************************************
函数功能：串口通信BBC校验与计算目标角度对应PWM值（浮点数版本）
入口参数：无
返回  值：无
**************************************************************************/
void Usart_Control()
{
    
}
