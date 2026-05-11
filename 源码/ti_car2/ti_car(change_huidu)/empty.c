/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "stdint.h"
#include "OLED.h"
#include "gray_track.h"
#include "motor.h"
#include "pid.h"
#include "No_Mcu_Ganv_Grayscale_Sensor_Config.h"
#include "Encoder.h"
#include "key.h"

unsigned short Anolog[8]={0};
unsigned short white[8]={2932,2056,2307,2416,3011,2929,2306,2187};
//unsigned short white[8]={900,900,900,900,900,900,900,900};
unsigned short black[8]={863,267,334,585,1262,1121,728,548};
unsigned short Normal[8];
unsigned char Digtal;
unsigned short Digtal_sign[8]={0,0,0,0,0,0,0,0};
No_MCU_Sensor sensor;
int encoder_count=0;
extern struct ENCODER  Encoder ;

bool flag1=0;

uint8_t response[100];//character strings for serial port and led
int main(void)
{
		timer_init();
		//NVIC_EnableIRQ(GRAY_OUT_INST_INT_IRQN);
		No_MCU_Ganv_Sensor_Init_Frist(&sensor);

    SYSCFG_DL_init();
		encoder_init();
		
		pid_init(&motorA, DELTA_PID, 300, 0, 58);
		pid_init(&motorB, DELTA_PID, 300, 0, 58);
		OLED_Init();
//		OLED_ShowSignedNum(2,9,0,4);	//distance
//		sprintf((char*)response, "area:");//track
//		OLED_ShowString(1,1,(char*)response);
//		sprintf((char*)response, "encoder:");//distance
//		OLED_ShowString(2,1,(char*)response);
//		sprintf((char*)response, "left:");//speed of left
//		OLED_ShowString(3,1,(char*)response);
//		sprintf((char*)response, "right:");//speed of right
//		OLED_ShowString(4,1,(char*)response);
    while (1) {
		No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor,Digtal_sign);
		Get_Anolog_Value(&sensor,Anolog);
	//此时打印的ADC的值，可用通过这个ADC作为黑白值的校准
	//也可以自己写按键逻辑完成一键校准功能
		sprintf((char *)response,"%d-%d    ",Anolog[0],Anolog[1]);
		OLED_ShowString(1,1,(char*)response);	
		sprintf((char *)response,"%d-%d    ",Anolog[2],Anolog[3]);
		OLED_ShowString(2,1,(char*)response);	
		sprintf((char *)response,"%d-%d   ",Anolog[4],Anolog[5]);
		OLED_ShowString(3,1,(char*)response);	
		sprintf((char *)response,"%d-%d   ",Anolog[6],Anolog[7]);
		OLED_ShowString(4,1,(char*)response);	
    }
}

void Pid_Time_INST_IRQHandler(void){
		//OLED_ShowSignedNum(1,6,(int)adc_getValue(),5);
		No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor,Digtal_sign);//get sensor's data
		Digtal=Get_Digtal_For_User(&sensor);//get sensor's data
		//DL_GPIO_togglePins(USERLED_PORT, USERLED_GREEN_PIN);
		if(flag1 == 1){
			motorA.now = Encoder.countl;//get speed
			motorB.now = Encoder.countr;//get speed
			//DL_GPIO_togglePins(USERLED_PORT, USERLED_GREEN_PIN);
			pid_control();
		}
}
void Huidu_Time_INST_IRQHandler(void){

		Key_Scan();
//		//DL_GPIO_togglePins(USERLED_PORT, USERLED_GREEN_PIN);
		if(flag1)
			DL_GPIO_clearPins(USERLED_PORT,USERLED_GREEN_PIN);	
		else
			DL_GPIO_setPins(USERLED_PORT,USERLED_GREEN_PIN);	
//			}
}