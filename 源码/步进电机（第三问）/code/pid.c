#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pid.h>
// PID 控制器结构体（离散版本）
//typedef struct {
//    double kp;           // 比例系数
//    double ki;           // 积分系数
//    double kd;           // 微分系数
//    double min_output;   // 输出最小值
//    double max_output;   // 输出最大值
//    double integral;     // 积分项累计值
//    double prev_error;   // 上一次误差
//} DiscretePIDController;

// 重置 PID 控制器状态
void PID_Reset(DiscretePIDController* pid) {
    pid->integral = 0.0;
    pid->prev_error = 0.0;
}

// 初始化 PID 控制器
void PID_Init(DiscretePIDController* pid, 
              double kp, double ki, double kd, 
              double min_output, double max_output) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->min_output = min_output;
    pid->max_output = max_output;
    PID_Reset(pid);
}



// 计算 PID 输出（离散版本）
double PID_Compute(DiscretePIDController* pid, double error) {
    // 比例项
    double p_term = pid->kp * error;
    
    // 积分项（简单累加）
    pid->integral += error;
    double i_term = pid->ki * pid->integral;
    
    // 微分项（使用误差差分）
    double derivative = error - pid->prev_error;
    double d_term = pid->kd * derivative;
    pid->prev_error = error;  // 更新历史误差
    
    // 计算总输出
    double output = p_term + i_term + d_term;
    
    // 应用输出限制
    if (output > pid->max_output) {
        output = pid->max_output;
    } else if (output < pid->min_output) {
        output = pid->min_output;
    }
    
    return output;
}

//// 二维坐标 PID 控制器
//typedef struct {
//    DiscretePIDController* x;
//    DiscretePIDController* y;
//} CoordinatePID;

// 计算二维坐标的 PID 控制输出
void coordinate_pid(double error_x, double error_y, 
                   CoordinatePID* controllers, 
                   double output[2]) {
    output[0] = PID_Compute(controllers->x, error_x);
    output[1] = PID_Compute(controllers->y, error_y);
}

