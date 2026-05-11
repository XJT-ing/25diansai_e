#include "motor.h"

//int Encoder_count1 = 0;
//int Encoder_count2 = 0;

//int speed_now;

#define AIN1(X)                                                                \
  ((X) ? (DL_GPIO_setPins(TB6612_PORT, TB6612_AIN2_PIN))                       \
       : (DL_GPIO_clearPins(TB6612_PORT, TB6612_AIN2_PIN)))
#define AIN2(X)                                                                \
  ((X) ? (DL_GPIO_setPins(TB6612_PORT, TB6612_AIN1_PIN))                       \
       : (DL_GPIO_clearPins(TB6612_PORT, TB6612_AIN1_PIN)))
#define BIN1(X)                                                                \
  ((X) ? (DL_GPIO_setPins(TB6612_PORT, TB6612_BIN1_PIN))                       \
       : (DL_GPIO_clearPins(TB6612_PORT, TB6612_BIN1_PIN)))
#define BIN2(X)                                                                \
  ((X) ? (DL_GPIO_setPins(TB6612_PORT, TB6612_BIN2_PIN))                       \
       : (DL_GPIO_clearPins(TB6612_PORT, TB6612_BIN2_PIN)))

void motorA_duty(int duty)
{
	if(duty >= 0)
	{
		AIN1(1);
		AIN2(0);
	}
	else
	{
		duty=-duty;
		AIN1(0);
		AIN2(1);
	}
	DL_TimerG_setCaptureCompareValue(PWM_INST,duty,DL_TIMER_CC_0_INDEX);
}

void motorB_duty(int duty)
{
	if(duty >= 0)
	{
		BIN1(1);
		BIN2(0);
	}
	else
	{
		duty=-duty;
		BIN1(0);
		BIN2(1);	
	}
	DL_TimerG_setCaptureCompareValue(PWM_INST,duty,DL_TIMER_CC_1_INDEX);
}


