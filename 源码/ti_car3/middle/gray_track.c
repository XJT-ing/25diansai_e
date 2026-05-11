#include "gray_track.h"
extern unsigned short Digtal_sign[8];
void track()                        //0123 4567 black=0,white=1
{
	static int left_flag=0;
	static int right_flag=0;
	int speed=15;
	int way;
	way=(Digtal_sign[0]*3 + Digtal_sign[1]*2 + Digtal_sign[2]*1 + Digtal_sign[3]*1 + Digtal_sign[4]*(-1) + Digtal_sign[5]*(-1) + Digtal_sign[6]*(-2) + Digtal_sign[7]*(-3) );
	
//
			
	if(way!=0){

			motor_target_set(speed+way,speed-way);//maxspeed=80
		
	}
	else if(way==0&&Digtal_sign[3]==0&&Digtal_sign[4]==0){//0000 0000 cross
		
		motor_target_set(speed,speed);

	}
	else{//1111 1111 outline
			motor_target_set(0.4*speed,speed);
	}
}

