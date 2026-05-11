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

/**************************************************************************
//函数功能：按键初始化
//入口参数：无
//返回  值：无 
**************************************************************************/
//void KEY_Init(void)
//{
//  HAL_PWR_EnableBkUpAccess();     //允许修改RTC 和后备寄存器
//    __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能后，PC13 PC14 PC15 才可以当普通IO用。
//	__HAL_RCC_BACKUPRESET_FORCE();//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//    __HAL_RCC_BACKUPRESET_RELEASE();
//} 
/**************************************************************************
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态 0：无动作 1：单击 2：双击 
**************************************************************************/
uint8_t click_N_Double(uint8_t time,bool key)
{
		static	uint8_t flag_key,count_key,double_key;	
		static	uint16_t count_single,Forever_count;
	  if(key==0)  Forever_count++;   //长按标志位未置1
     else        Forever_count=0;
		if(0==key&&0==flag_key)		flag_key=1;	
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;	
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//双击执行的指令
				}
		}
		if(1==key)			flag_key=0,count_key=0;
		
		if(1==double_key)
		{
			count_single++;
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//单击执行的指令
			}
			if(Forever_count>time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
}
/**************************************************************************
函数功能：按键扫描
入口参数：无
返回  值：按键状态 0：无动作  
**************************************************************************/
uint8_t click(void)
{     
	    int  temp;
			static uint8_t flag_key=1;//按键按松开标志
//			if(flag_key&&(KEY_S==0||KEY_P==0||KEY_J==0||KEY_M==0||KEY_X==0))
//			{
//			flag_key=0;
////						if(KEY_S==0)  temp=1;
////			else 	if(KEY_P==0)  temp=2;
////			else 	if(KEY_X==0)  temp=3;
////			else 	if(KEY_J==0)  temp=4;
////			else 	if(KEY_M==0)  temp=5;				
//			return temp;	  // 按键按下
//			}
//			else if(1==KEY_S&&1==KEY_P&&1==KEY_J&&1==KEY_M&&1==KEY_X)			flag_key=1;
			return 0;//无按键按下
}
/**************************************************************************
函数功能：长按检测
入口参数：无
返回  值：按键状态 0：无动作 1：长按2s
**************************************************************************/
uint8_t Long_Press(int key)
{
			static uint16_t Long_Press_count,Long_Press;
	    if(Long_Press==0&&key==0)  Long_Press_count++;   //长按标志位未置1
      else                       Long_Press_count=0; 
		  if(Long_Press_count>100)		
			{
				Long_Press=1;	
				Long_Press_count=0;
				return 1;
			}				
			 if(Long_Press==1)     //长按标志位置1
			{
				  Long_Press=0;
			}
			return 0;
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key_Scan(void)
{	   
			extern int flag1;
			bool temp;
			static bool key1_flag=0;
			static bool key1_temp=0;
			static bool key2_flag=0;
			static bool key2_temp=0;
			static bool key4_flag=0;
			static bool key4_temp=0;
//			if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)){
//				
//					start_flag=1;
////					encoder_count=0;
////					HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
////					HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
//			}
			
//			if(!HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)){
////					start_flag=0;
//					stop_flag=1;
////					motorA_duty(0);
////					motorB_duty(0);
////					HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
////					HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
//			}
//////////////KEY0////////	
			temp=click_N_Double(20,DL_GPIO_readPins(KEY_PORT,KEY_KEY1_PIN));//key1
//			if(temp==1){
//	//			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//					Emm_V5_Pos_Control(0, 1, 10, 0, 96, 0, 0, huart5);
//				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
//			}else if(temp==2){
//	//			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
//					Emm_V5_Pos_Control(0, 0, 10, 0, 96, 0, 0, huart5);
//				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
//			}
			if(temp==1){
				DL_GPIO_setPins(USERLED_PORT,USERLED_GREEN_PIN);			
			}
//////////////KEY2////////			
				key2_temp=key2_flag;
				key2_flag=DL_GPIO_readPins(KEY_PORT,KEY_KEY2_PIN);//key1
			if(key2_temp==1&&key2_flag==0){
				//DL_GPIO_setPins(USERLED_PORT,USERLED_GREEN_PIN);	
				flag1=1;
			}
//////////////KEY4////////				
			key4_temp=key4_flag;
			key4_flag=DL_GPIO_readPins(KEY_PORT,KEY_KEY4_PIN);//key2
			if(key4_temp==1&&key4_flag==0){
				//DL_GPIO_clearPins(USERLED_PORT,USERLED_GREEN_PIN);	
				flag1=0;
				motorA_duty(0);
				motorB_duty(0);
			}
}
