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
#include "key.h"
#include "usart.h"
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key_Scan(void)
{	  
			//extern int three_flag1;
			static uint8_t key0_flag=0;
			static uint8_t key0_temp=0;
			static uint8_t key1_flag=0;
			static uint8_t key1_temp=0;
			static uint8_t key2_flag=0;
			static uint8_t key2_temp=0;
			uint8_t cmd[32] = {0};
  
//////////////KEY0////////	
				key0_temp=key0_flag;
				key0_flag=HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin);//key1
			if(key0_temp==1&&key0_flag==0){
//					Emm_V5_Pos_Control(0, 1, 10, 0, 0, 1, 0, huart5);
//					Emm_V5_Pos_Control(0, 1, 10, 0, 0, 1, 0, huart6);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);	
				HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
				sprintf((char*)cmd, "02");
				HAL_UART_Transmit(&huart1, cmd, 2, 100);
			}
//////////////KEY1////////			
				key1_temp=key1_flag;
				key1_flag=HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);//key1
			if(key1_temp==1&&key1_flag==0){
	//			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//					Emm_V5_Pos_Control(0, 1, 30, 0, 480, 0, 0, huart5);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
				sprintf((char*)cmd, "03");				
				HAL_UART_Transmit(&huart1, cmd, 2, 100);
			}
//////////////KEY2////////				
			key2_temp=key2_flag;
			key2_flag=HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);//key2
			if(key2_temp==1&&key2_flag==0){
//	//			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//					Emm_V5_Pos_Control(0, 0, 30, 0, 480, 0, 0, huart5);
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
				sprintf((char*)cmd, "04");				
				HAL_UART_Transmit(&huart1, cmd, 2, 100);
				Emm_V5_Stop_Now(0,0, huart5);
			}
}
