// SIMULATION FOR ONTARIO AIRPORT PICKUP ZONE
// Yuxuan(Leo) Li
// SID: 861045931

#include <utility> 
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <queue> 
#include "cpp.h"


using namespace std; 


#define NUMCARS 10              // the number of cars in this simulation
#define NUMROADS 120              //number of roads in this simulation
#define SIMULATIONTIME 6000         //TIME OF THE SIMULATION
#define LIGHT 50                     //the placement of signal light

facility_set road("road",NUMROADS);
 
int NumCars = NUMCARS; //number of total cars in the simulation variable
int roadNum = NUMROADS; //var for number of total roads in simulation
double Stime = SIMULATIONTIME; //var for the total simulation time simulated for

int SPEEDUP = 0;
int SLOWDOWN = 0;  

int TarSpeed = 0;  //target speed for each car

double D[NUMROADS]; //D[i] stores the departure time of current car in cell[i]
//int Cars[NumCars]; 
int SpeedArray[NUMROADS];  

//function declarations
double WaitTime(int speed);
double value = 0; 
bool ShouldSlowDown(int speed, int loc); 
void spawn(int startingloc, int startinglocR, int MSPEED, double TChange);
void TrafficLight();
int count = 0; 


void TrafficLight(){
	create ("signal");
	
	while(1){

		hold(10); //yellow light 
		road[LIGHT].reserve();
		printf("\n");
		printf("************************************************");
		printf("\n");
		printf("!!!!!!!!!!!!!!!!!!!!RED!!!!!!!!!!!!!!!!!!!!!!!!!");
		printf("\n");
		printf("************************************************");
		printf("\n");
		hold(uniform(0,900)); //red light
		road[LIGHT].release();
		hold(expntl(120)); 
	}
}


extern "C" void sim()		// main process

{
	

	if (NumCars == roadNum){
		printf("ERROR, TOO MANY CARS FOR THE NUMBER OF ROADS IN THIS SIMULATION");
		printf("\n");
		exit(-1);
	}
	printf("%2i",roadNum);
	printf(" LOCATIONS \n");
	printf("%2i", NumCars);
	printf(" CAR(S) \n");
	printf(" TIME RAN FOR: ");
	printf("%5f", Stime);
	printf("\n");
	
	create("sim");
	reseed(NIL,3);
	//trace_on();

	pair<int,int> L;
	
	L.second = (NumCars*2) - 1; 
	L.first = L.second - 1; 
	
	//TrafficLight(); 
	int Sfirst;  
	double TTChange = uniform(60,120); 
	for (int i = 0; i < NumCars; i++){
		
		Sfirst = uniform_int(2,5);	
		spawn(L.first,L.second,Sfirst,TTChange);
		printf("%1i",i);
		printf("car goes at speed ");
		printf("%1i",Sfirst);
		printf("\n");
		L.first = L.first - 2; 
		L.second = L.second - 2;
	}
	hold(SIMULATIONTIME);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("%3d",count/10);
	printf("\n");
	printf("\n");
	printf("\n");


}

double TimeChangeCnt = 0; 
bool slowdown = false; 
void spawn(int startingloc, int startinglocR, int SPEED, double TChange)
//spawns a new car 
{
	create("car");
	trace_on();
	
	bool INMOTION = false; 
	bool NOMOTION = true;
	 
	int MSPEED = SPEED;	
	unsigned int speed = 0; 
	
	double SPEEDCHANGE = TChange; 

	int cnt = 0; 
	int location = startingloc; 
	int locationR = startinglocR;
	

	road[location].reserve();
	road[locationR].reserve();
	bool first = true;
	int d = 0;  

while(1){


 	if(INMOTION){
		
		if(clock >= TChange){
			TimeChangeCnt = 0; 
			TChange += uniform(60,120);
			MSPEED = uniform_int(2,5); 
		}


	
		if (MSPEED < speed){
			slowdown = true;
		}
		if (speed == MSPEED){
			speed = speed; 
			slowdown = false;
		}
		
		printf("\n");
		printf("CAR ");
		printf("%2i", identity());
		printf("\n");
		printf("is going at SPEED: ");
		printf("%1i", speed);	
		printf("\n");
		
		printf("\n");
		SpeedArray[location] = speed;

			
		locationR = locationR + 1;
		if (locationR >= NUMROADS){
			locationR = 0;
		}
		
		hold(WaitTime(speed));
		D[location] = WaitTime(speed);
		road[locationR].reserve();	
		road[location].release();
		
		location = location + 1;
		if(location >= NUMROADS){
			location = 0;
			count++; 
		}
		cnt++;

		if (ShouldSlowDown(speed,locationR) || slowdown){
			cnt = 0; 
			printf("\n");
			printf("SLOWING DOWN NOW SIR");
			printf("\n");
			speed = speed - 1;
			printf("%1i", speed);
			if(speed == 0){
				cnt = 0;
				road[locationR].release(); 
				NOMOTION = true;
				INMOTION = false;
			}
		}
		
		if (speed < MSPEED && !ShouldSlowDown(speed,locationR)){
			slowdown = false;
			if (cnt > 1 && speed != 5){
				cnt = 0;
				speed++;
			}
			else if (cnt > 1 && speed == 5){
				cnt = 0;
				speed = 5;
			}
			
		}
		if (MSPEED < speed){
			slowdown = true;
		}
		if (speed == MSPEED){
			speed = speed; 
			slowdown = false;
		}
	

	}
	 
	if(NOMOTION){
	

		if(clock >= TChange){
			TimeChangeCnt = 0; 
			TChange += uniform(60,120);
			MSPEED = uniform_int(2,5);
		}
		
		if(!ShouldSlowDown(1,locationR)){
			speed = 1;
			if(first){
				first = false; 
				locationR = locationR + 1;
				road[locationR].reserve();
			}
			else {
				road[locationR].reserve();
			}
			NOMOTION = false;
			INMOTION = true;
			printf("\n");
			printf("CAR ");
			printf("%2i", identity());
			printf("\n");
			printf("is in motion");
			printf("\n");
		}
		else{
			printf("\n");
			printf("CAR ");
			printf("%2i", identity());
			printf("\n");
			printf("has no motion");
			printf("\n");
			hold(1);
			NOMOTION = true;
			INMOTION = false; 
		}
	}
}
		

}


double WaitTime(int speed){
	double Htime = 0;
	
	if(speed == 0){
		exit(-1);
	} 
	if (speed == 1){
		Htime = (1.5);
	}
	if (speed == 2){
		Htime = (0.916666);
	}
	if (speed == 3){
		Htime =(0.5);
	}
	if (speed == 4){
		Htime = (0.33333);
	}
	if (speed == 5){
		Htime = (0.25); 
	}
	return Htime; 
}


bool ShouldSlowDown(int speed, int loc){
	
	int next= loc+1; 
	int next2=loc+2;
	int next3=loc+3;
	int next4=loc+4;
	int next5=loc+5;
	int next6=loc+6;
	int next7=loc+7;
	int next8=loc+8;
	
	if (next >= NUMROADS){
		next = next - NUMROADS; 
	}
	if (next2 >= NUMROADS){
		next2 = next2 - NUMROADS; 
	}
	if (next3 >= NUMROADS){
		next3 = next3 - NUMROADS;
	}
	if (next4 >= NUMROADS){
		next4 = next4 - NUMROADS;
	}
	if (next5 >= NUMROADS){
		next5 = next5 - NUMROADS;
	}
	if (next6 >= NUMROADS){
		next6 = next6 - NUMROADS; 
	}
	if (next7 >= NUMROADS){
		next7 = next7 - NUMROADS;
	}
	if (next8 >= NUMROADS){
		next8 = next8 - NUMROADS;
	}
	
	if (speed == 0){
		return false;
	}
	else if (speed == 1 || speed == 2){
		if (!road[next].num_busy()&& 
                    !road[next2].num_busy()){
		    return false;
		}
		else {
			return true; 
		}
	}
	else if (speed == 3){
		if (!road[next].num_busy()&& 
		    !road[next2].num_busy()&& 
		    !road[next3].num_busy()&& 
	            !road[next4].num_busy() ){
		    return false; 
		}
		else {
			return true; 
		}
	}
	else if (speed == 4){
		if (!road[next].num_busy()&& 
                    !road[next2].num_busy()&& 
                    !road[next3].num_busy()&& 
                    !road[next4].num_busy()&&
                    !road[next5].num_busy()&& 
	            !road[next6].num_busy()){
		    return false; 
		}
		else {
			return true; 
		}
	}
	else if (speed == 5){
		if (!road[next].num_busy()&& 
                    !road[next2].num_busy()&& 
                    !road[next3].num_busy()&& 
                    !road[next4].num_busy()&&
                    !road[next5].num_busy()&& 
	            !road[next6].num_busy()&&
		    !road[next7].num_busy()&& 
                    !road[next8].num_busy()){
		    return false; 
		}

		else {
			return true; 
		}
	}
}


