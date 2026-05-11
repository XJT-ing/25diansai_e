#ifndef _motor_h
#define _motor_h
#pragma once  //!防止头文件被重复包含
#include "ti_msp_dl_config.h"
void motorA_duty(int duty);
void motorB_duty(int duty);
void timer_init(void);
//extern int Encoder_count1, Encoder_count2;
//extern int speed_now;

#endif
