#ifndef __PID_h_
#define __PID_h_
#include "ti_msp_dl_config.h"

enum
{
  POSITION_PID = 0,  // 位置式
  DELTA_PID,         // 增量式
};

typedef struct
{
	int target;	
	int now;
	int error[3];		
	int p,i,d;
	int pout, dout, iout;
	int out;   
	
	uint32_t pid_mode;

}pid_t;

void pid_cal(pid_t *pid);
void pid_control(void);
void pid_init(pid_t *pid, uint32_t mode, int p, int i, int d);
void motor_target_set(int spe1, int spe2);
void pidout_limit(pid_t *pid);

extern pid_t motorA;
extern pid_t motorB;
extern pid_t angle;
#endif
