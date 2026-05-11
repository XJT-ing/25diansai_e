#include "gray_track.h"
extern unsigned short Digtal_sign[8];
void track()                        //0123 4567 black=0,white=1
{
	static int left_flag=0;
	static int right_flag=0;
	int speed=20;
	int way;
	way=(Digtal_sign[0]*5 + Digtal_sign[1]*4 + Digtal_sign[2]*3 + Digtal_sign[3]*2 + Digtal_sign[4]*(-2) + Digtal_sign[5]*(-3) + Digtal_sign[6]*(-4) + Digtal_sign[7]*(-5) );
	
	if(Digtal_sign[0]==0&&Digtal_sign[7]==1){//0xxx xxx1 left_flag
			left_flag=1;
			right_flag=0;
	}
	else if(Digtal_sign[0]==1&&Digtal_sign[7]==0){//1xxx xxx0 right_flag
			left_flag=0;
			right_flag=1;
	}
			
	if(way!=0){

			motor_target_set(speed+way,speed-way);//maxspeed=80
		
	}
	else if(way==0&&Digtal_sign[3]==0&&Digtal_sign[4]==0){//0000 0000 cross
		
		motor_target_set(speed,speed);

	}
	else{//1111 1111 outline
		
		if(left_flag==1){
			
			motor_target_set(-speed,speed);
			
		}else if(right_flag==1){
			
			motor_target_set(speed,-speed);
			
		}
	}
}

