#include "Encoder.h"


struct ENCODER  Encoder ;
extern int encoder_count;

void encoder_init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(EncoderA_INT_IRQN);
    NVIC_ClearPendingIRQ(EncoderB_INT_IRQN);
	NVIC_EnableIRQ(EncoderA_INT_IRQN);
    NVIC_EnableIRQ(EncoderB_INT_IRQN);
}

void timer_init(void)
{
    //定时器中断
	NVIC_ClearPendingIRQ(TIME_End_INST_INT_IRQN);
	NVIC_EnableIRQ(TIME_End_INST_INT_IRQN);
	NVIC_ClearPendingIRQ(Pid_Time_INST_INT_IRQN);
	NVIC_EnableIRQ(Pid_Time_INST_INT_IRQN);
	NVIC_ClearPendingIRQ(Huidu_Time_INST_INT_IRQN);
	NVIC_EnableIRQ(Huidu_Time_INST_INT_IRQN);
}

/**/
/********************************************
函数功能：中断读取编码器
参数：无
返回值：无
********************************************/
void GROUP1_IRQHandler(void)
{
    /* 合并GPIO状态读取 */
    const uint32_t encoderA_pins = EncoderA_PIN1_PIN | EncoderA_PIN2_PIN;
    const uint32_t encoderB_pins = EncoderB_PIN3_PIN | EncoderB_PIN4_PIN;
    
    // 原子化操作：单次读取所有相关引脚状态
    uint32_t gpioA = DL_GPIO_getEnabledInterruptStatus(EncoderA_PORT, encoderA_pins);
    uint32_t gpioB = DL_GPIO_getEnabledInterruptStatus(EncoderB_PORT, encoderB_pins);

    /* 编码器A处理（优化为位操作+状态合并） */
    if(gpioA) {
        // 同时读取两个引脚电平（减少IO操作）
        bool pin1State = DL_GPIO_readPins(EncoderA_PORT, EncoderA_PIN1_PIN);
        bool pin2State = DL_GPIO_readPins(EncoderA_PORT, EncoderA_PIN2_PIN);

        // 正交解码逻辑优化
        switch(gpioA) {
            case EncoderA_PIN1_PIN:
                Encoder.temp_countl += (pin2State ? -1 : 1);
                break;
            case EncoderA_PIN2_PIN:
                Encoder.temp_countl += (pin1State ? 1 : -1);
                break;
            default: // 处理意外情况
                break;
        }
    }

    /* 编码器B处理（相同优化模式） */
    if(gpioB) {
        bool pin3State = DL_GPIO_readPins(EncoderB_PORT, EncoderB_PIN3_PIN);
        bool pin4State = DL_GPIO_readPins(EncoderB_PORT, EncoderB_PIN4_PIN);

        switch(gpioB) {
            case EncoderB_PIN4_PIN:
                Encoder.temp_countr += (pin3State ? 1 : -1);
                break;
            case EncoderB_PIN3_PIN:
                Encoder.temp_countr += (pin4State ? -1 : 1);
                break;
            default:
                break;
        }
    }

    /* 统一清除中断标志 */
    DL_GPIO_clearInterruptStatus(EncoderA_PORT, encoderA_pins);
    DL_GPIO_clearInterruptStatus(EncoderB_PORT, encoderB_pins);
}


void TIME_End_INST_IRQHandler(void)
{
    if( DL_TimerG_getPendingInterrupt(TIME_End_INST) == DL_TIMER_IIDX_ZERO )
	{
//        Encoder.countr = Encoder.temp_countr;
//        Encoder.temp_countr = 0 ;
//        Encoder.countl = Encoder.temp_countl;
//        Encoder.temp_countl = 0 ;
//				encoder_count+=Encoder.countl;//calculate distance
//				encoder_count+=Encoder.countr;//calculate distance
    }
	
}

