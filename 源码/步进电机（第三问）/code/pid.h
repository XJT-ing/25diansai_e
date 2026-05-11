#ifndef PID_H
#define PID_H

// PID 控制器结构体声明
typedef struct DiscretePIDController {
    double kp, ki, kd;
    double min_output, max_output;
    double integral;
    double prev_error;
} DiscretePIDController;

// 坐标 PID 结构体声明
typedef struct {
    DiscretePIDController* x;
    DiscretePIDController* y;
} CoordinatePID;

// 全局变量声明（在其他文件中定义）
extern CoordinatePID coord_pid;

// 函数声明
void PID_Init(DiscretePIDController* pid, double kp, double ki, double kd, 
              double min_output, double max_output);
void PID_Reset(DiscretePIDController* pid);
double PID_Compute(DiscretePIDController* pid, double error);
void coordinate_pid(double error_x, double error_y, CoordinatePID* controllers, double output[2]);

#endif
