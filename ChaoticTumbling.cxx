#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "gnuplot.cxx"
using namespace std; 

const double PI = 3.141592653589;

int main()
{
	//Solar System 
	double dt = 0.001;		//time step - Hyperion years
	double T = 15;			//total time - Hyperion years
	int N = T/dt;	 	//number of steps - not points

	double x[N+1], y[N+1]; 		//array of x co-ordinates									//
	double vx[N+1], vy[N+1]; 	//velocity values											//
	double r[N+1]; 				//radius of orbit (this varies in elliptical motion)		//
																							//	all arrays have N+1 components as N represents "steps", thus requiring N+1 "points" to graph
	double theta[N+1]; 			//angle "theta" for position in orbit						//
	double angVelocity[N+1]; 	//angular velocity											//
	double time[N+1];			//array for storing values of t - Hyperion Years			//
		time[0] = 0;			//inital time, *this cannot be modified
		
	//other initial conditions
	r[0] = 1; 					//initial radius - Hyperion unit(s)			
	x[0] = r[0];				//starting position is initial length of radius on the x axis - Hyperion Unit(s)
	y[0] = 0;					// ^
	vx[0] = 0;					//cannot be modified unless inital position is on the y-axis
	vy[0] = 8;					//Since we are starting on the x-axis, vy is our inital velocity
	theta[0] = 0;				//Inital value of theta - radians
	angVelocity[0] = 0;			//inital angular velocity - radians / Hyperion Year

	//iteration
	int i = 0;
	while(i <= N){
		
		//time
		time[i+1] = (i+1)*dt;
		
		//velocity components
		vx[i+1] = vx[i] - ((4*PI*PI*x[i])/(r[i]*r[i]*r[i]))*dt;
		vy[i+1] = vy[i] - ((4*PI*PI*y[i])/(r[i]*r[i]*r[i]))*dt;
		
		//cartesian co-ordinates
		x[i+1] = x[i] + (vx[i+1]*dt);
		y[i+1] = y[i] + (vy[i+1]*dt);
		
		//angular velocity
		angVelocity[i+1] = angVelocity[i] - (((12*PI*PI*((x[i]*sin(theta[i]))-(y[i]*cos(theta[i])))*((x[i]*cos(theta[i]))+(y[i]*sin(theta[i])))))/(r[i]*r[i]*r[i]*r[i]*r[i]))*dt;
		
		//theta
		theta[i+1] = theta[i] + angVelocity[i+1]*dt;
		
		//theta reset
		if(theta[i+1] > PI){
			theta[i+1] = -PI;
		}
		if(theta[i+1] < -PI){
			theta[i+1] = PI;
		}
				
		//radius 
		r[i+1] = sqrt((x[i+1]*x[i+1]) + (y[i+1]*y[i+1]));
		//cout << r[i] << "\n";
		
			
		i++;	

	}
	
	gnuplot_one_function ("Hyperion orbiting Saturn", "lines", "x", "y", x, y, N+1);
	gnuplot_one_function ("Theta Vs. Time", "lines", "Time / Hyperion Years", "Theta", time, theta, N+1);
	gnuplot_one_function ("Angular Velocity Vs. Time", "lines", "Time / Hyperion Years", "AngVelocity", time, angVelocity, N+1);
	
	return 0;
}

