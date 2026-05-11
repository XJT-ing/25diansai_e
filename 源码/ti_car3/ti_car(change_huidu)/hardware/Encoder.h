#pragma once  //!防止头文件被重复包含
#include "ti_msp_dl_config.h"
struct ENCODER
{
    volatile long long temp_countl; //保存实时计数值
    volatile long long temp_countr; //保存实时计数值
    int countl;         			//根据定时器时间更新的计数值
    int countr;
};
extern struct ENCODER  Encoder ;


void encoder_init(void);
void timer_init(void);