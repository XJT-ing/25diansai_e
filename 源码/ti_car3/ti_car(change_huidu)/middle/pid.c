#include "pid.h"
#include "stdint.h"
#include "No_Mcu_Ganv_Grayscale_Sensor_Config.h"
#include "gray_track.h"
#include "motor.h"
#define MAX_DUTY  80000

pid_t motorA;
pid_t motorB;
pid_t angle;

extern No_MCU_Sensor sensor;
void datavision_send()  // 上位机波形发送函数
{
//	// 数据包头
//	uart_sendbyte(UART_1, 0x03);
//	uart_sendbyte(UART_1, 0xfc);

//	// 发送数据
//	uart_sendbyte(UART_1, (uint8_t)motorA.target);  
//	uart_sendbyte(UART_1, (uint8_t)motorA.now);
////	uart_sendbyte(UART_1, (uint8_t)motorB.target);  
////	uart_sendbyte(UART_1, (uint8_t)motorB.now);
//	// 数据包尾
//	uart_sendbyte(UART_1, 0xfc);
//	uart_sendbyte(UART_1, 0x03);
}


void pid_init(pid_t *pid, uint32_t mode, int p, int i, int d)
{
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
}

void motor_target_set(int spe1, int spe2)
{

		motorA.target = spe1;

		motorB.target = spe2;

}


void pid_control()
{
	// 角度环
	// 1.设定目标角度
	//angle.target = -20;
	// 2.获取当前角度
	//angle.now = yaw_Kalman;
	// 3.PID控制器计算输出
	//pid_cal(&angle);
	
	// 速度环
	// 1.根据灰度传感器信息 设定目标速度
  	track();

	// 3.输入PID控制器进行计算
	pid_cal(&motorA);
	pid_cal(&motorB);
	// 电机输出限幅
	pidout_limit(&motorA);
	pidout_limit(&motorB);
	// 4.PID的输出值 输入给电机
	motorA_duty(motorA.out);
	motorB_duty(motorB.out);
	
//	datavision_send();
}
void pid_cal(pid_t *pid)
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// 输出限幅
//	if(pid->out>=MAX_DUTY)	
//		pid->out=MAX_DUTY;
//	if(pid->out<=0)	
//		pid->out=0;
	
}

void pidout_limit(pid_t *pid)
{
	// 输出限幅
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=-MAX_DUTY)	
		pid->out=-MAX_DUTY;
//	if(pid->out<=10)	
//		pid->out=0;
}


