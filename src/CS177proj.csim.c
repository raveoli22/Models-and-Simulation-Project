// Simulation of the Hertz airport shuttle bus, which picks up passengers
// from the airport terminal building going to the Hertz rental car lot.
// Yuxuan(Leo) Li
// SID: 861045931



#include "cpp.h"
#include <utility> 
#include <string.h>
#include <math.h>

using namespace std; 


#define TINY 1.e-20		// a very small time period
#define NUMCARS 2              // the number of cars in this simulation
#define NUMROADS 100              //number of roads in this simulation
#define SIMULATIONTIME 50          //TIME OF THE SIMULATION


//int steps = 0;
 
facility_set road("road",NUMROADS); //facility set for the roads in this simulation
int NumCars = NUMCARS; //number of total cars in the simulation variable
int roadNum = NUMROADS; //var for number of total roads in simulation
double time = SIMULATIONTIME; //var for the total simulation time simulated for

int SPEEDUP = 0;
int SLOWDOWN = 0;  

int SPEED = 0; //current speed of the car
int TarSpeed = 0;  //target speed for each car

double D[NUMROADS]; //D[i] stores the departure time of current car in cell[i]  

//function declarations
double WaitTime(int speed);
bool ShouldSlowDown(int speed, int loc); 

void spawn(int startingloc, int startinglocR, int Dspeed, double TChange);
void TrafficLight();

void TrafficLight(){
	create ("signal");
	//trace_on();
	
	while(1){
		hold(expntl(120));
		road[40].reserve();
		hold(uniform(0,90)); //red light
		road[40].release();
		hold(10); //yellow light 
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
	printf("%5f", time);
	printf("\n");
	
	create("sim");
	trace_on();

	pair<int,int> L;
	
	L.second = (NumCars*2) - 1; 
	L.first = L.second - 1; 
	int DesignatedSpeed = 5;
	
	//**********************************************
	//add process for changing designated speed here
	//**********************************************
	//**********************************************
	
	//TrafficLight();
	double TTChange = 0; 
	for (int i = 0; i < NumCars; i++){
		//trace_on();
		spawn(L.first,L.second,DesignatedSpeed,TTChange);
		L.first = L.first - 2; 
		L.second = L.second - 2;
		DesignatedSpeed = DesignatedSpeed - 2; 
	}
	hold(SIMULATIONTIME);
	report();

}

double TimeChangeCnt = 0; 
bool INMOTION = false;
bool NOMOTION = true;
bool first = false; 
void spawn(int startingloc, int startinglocR,int Dspeed,double TChange)      //spawns a car, give it location, give it R, and give it next location to be
{
	create("car");
	//trace_on();
	
	int speed = 0; 
	int cnt = 0;
	int TOPSPEED = Dspeed; 
	double SpeedChange = TChange;
	int location = startingloc; 
	int locationR = startinglocR;
	

	road[location].reserve();
	road[locationR].reserve();

	
	if (speed == 0 && speed != TOPSPEED){
		speed = 1;
		
		if (ShouldSlowDown(1,locationR)){
			printf("\n");
			printf("SLOWING DOWN NOW SIR 1");
			printf("\n");
			speed = 0;
			INMOTION = false;
			NOMOTION = true;
			first = true; 
		}
		else {
			locationR = locationR + 1; 
			road[locationR].reserve();
			INMOTION = true;
			NOMOTION = false;
		}
	}

while(1){

 	if(INMOTION){
		
		TimeChangeCnt++;
		if(TimeChangeCnt == SpeedChange){
			TimeChangeCnt = 0;
			SpeedChange = uniform(60,120);
			TOPSPEED = uniform(2,5);  
		}
	
		printf("SPEED: ");
		printf("%1i", speed);	
		printf("\n");

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
		}
		cnt++;

		if (ShouldSlowDown(speed,locationR)){
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
		
		if (speed != TOPSPEED && !ShouldSlowDown(speed,locationR)){
			if (cnt > 1){
				cnt = 0;
				speed++;
			}
		}
		

	}
	 
	if(NOMOTION){
		
		TimeChangeCnt++;
		if (TimeChangeCnt == SpeedChange){
			TimeChangeCnt = 0; 
			SpeedChange = uniform(60,120);
			TOPSPEED = uniform(2,5);
		}

		
		if(!ShouldSlowDown(1,locationR)){
			speed = 1;
			if(first){
				first = false; 
				locationR = locationR + 1;
				road[location].reserve();
			}
			else {
				road[locationR].reserve();
				//road[3].release();
			}
			NOMOTION = false;
			INMOTION = true;
			printf("\n");
			printf("back in motion");
			printf("\n");
		}
		else{
			printf("\n");
			printf("still no motion");
			printf("\n");
			hold(1);
			INMOTION = false;
			NOMOTION = true;
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
	else if (speed == 1){
		if (!road[next].num_busy()&& 
                    !road[next2].num_busy()){
		    return false;
		}
		else {
			return true; 
		}
	}
	else if (speed == 2 || speed == 3){
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


