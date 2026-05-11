#ifndef __KEY_H
#define __KEY_H	 
#include "stdint.h"
#include "motor.h"
#include "ti_msp_dl_config.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

//void KEY_Init(void);          //按键初始化
uint8_t click_N_Double (uint8_t time,bool key);  //单击按键扫描和双击按键扫描
uint8_t click(void);               //单击按键扫描
uint8_t Long_Press(int key);           //长按扫描  
void Key_Scan(void);          //

#endif  
