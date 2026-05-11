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
#include "stdint.h"
unsigned short Anolog[8]={0};
unsigned short white[8]={2222,1547,1644,1782,2503,2410,1860,1737};
unsigned short black[8]={1000,1000,1000,1000,1000,1000,1000,1000};
unsigned short Normal[8];
unsigned short Digtal_sign[8]={0,0,0,0,0,0,0,0};
No_MCU_Sensor sensor;
int encoder_count=0;
extern struct ENCODER  Encoder ;

bool flag1=0;
bool stopflag=0;
unsigned short times=0;

uint8_t response[32];//character strings for serial port and led
int main(void)
{
		timer_init();
		SYSCFG_DL_init();
//		No_MCU_Ganv_Sensor_Init_Frist(&sensor);
		No_MCU_Ganv_Sensor_Init(&sensor,white,black);
    
		encoder_init();
		
		pid_init(&motorA, POSITION_PID, 20, 0, 2);
		pid_init(&motorB, POSITION_PID, 20, 0, 2);
		OLED_Init();
//		sprintf((char*)response, "area:");//track
//		OLED_ShowString(1,1,(char*)response);
		sprintf((char*)response, "encoder:");//distance
		OLED_ShowString(2,1,(char*)response);
		sprintf((char*)response, "times:");//speed of left
		OLED_ShowString(3,1,(char*)response);
		sprintf((char*)response, "start:");//speed of right
		OLED_ShowString(4,1,(char*)response);
    while (1) {
			No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor,Digtal_sign);//get sensor's data			
//		sprintf((char *)response,"%d%d%d%d%d%d%d%d",Digtal_sign[0],Digtal_sign[1],Digtal_sign[2],Digtal_sign[3],Digtal_sign[4],Digtal_sign[5],Digtal_sign[6],Digtal_sign[7]);
//		OLED_ShowString(1,6,(char*)response);//gray of track
		OLED_ShowSignedNum(1,1,12800*times,5);	//distance	
		OLED_ShowSignedNum(2,9,encoder_count,5);	//distance
		OLED_ShowSignedNum(3,6,times,2);	//times
		OLED_ShowSignedNum(4,7,flag1,2);	//start
			//12400 distance of one circule
    }
}

void Pid_Time_INST_IRQHandler(void){
//		No_Mcu_Ganv_Sensor_Task_Without_tick(&sensor,Digtal_sign);//get sensor's data
		if(flag1 == 1){
			motorA.now = Encoder.countl;//get speed
			motorB.now = Encoder.countr;//get speed
			pid_control();
		}
}

			bool temp;
			bool key1_flag=0;
			bool key1_temp=0;
			bool key2_flag=0;
			bool key2_temp=0;
			bool key4_flag=0;
			bool key4_temp=0;

void Key_Scan(void)
{	   
	//////////////KEY1////////开始按键			
				key1_temp=key1_flag;
				key1_flag=DL_GPIO_readPins(KEY_PORT,KEY_KEY1_PIN);//key1
			if(key1_temp==1&&key1_flag==0){
				flag1=1;
			}
//////////////KEY2////////圈数按键			
				key2_temp=key2_flag;
				key2_flag=DL_GPIO_readPins(KEY_PORT,KEY_KEY2_PIN);//key1
			if(key2_temp==1&&key2_flag==0){	
				times=(times%5+1);
				flag1=0;
			}
//////////////KEY4////////stop按键				
			key4_temp=key4_flag;
			key4_flag=DL_GPIO_readPins(KEY_PORT,KEY_KEY4_PIN);//key2
			if(key4_temp==1&&key4_flag==0){	
				flag1=0;
				encoder_count=0;
				motorA_duty(0);
				motorB_duty(0);
			}
}

void Huidu_Time_INST_IRQHandler(void){

		Key_Scan();
		if(encoder_count>=(12800*times)){
			flag1=0;
			motorA_duty(0);
			motorB_duty(0);
		}
}