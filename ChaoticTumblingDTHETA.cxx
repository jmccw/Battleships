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
const double Ti = 11;
const double dti = 0.0005;
const double vy0i = 2*PI;

double hyperion(double q, int j){	//copied from main in previous program, only what needs to be run. did this to save memory - but not time unfortunately
									//q is the initial value for theta, j is the iteration which is called at the end - unfortuantely the only viable to do this. -Too many arrays.
	//Solar System 
	int N; 			//number of points 
	double T, dt; 	//time and step size
	dt = dti;		//Hyperion years
	T = Ti;			//Hyperion years
	N = T/dt;	 	//number of steps - not points

	double x[N+1], y[N+1]; 		//array of x co-ordinates									//
	double vx[N+1], vy[N+1]; 	//velocity values											//
	double r[N+1]; 				//radius of orbit (this varies in elliptical motion)		//
																							//	all arrays have N+1 components as N represents "steps", thus requiring N+1 "points" to graph
	double theta[N+1]; 			//angle "theta" for position in orbit						//
	double angVelocity[N+1]; 	//angular velocity											//
	//double time[N+1];			//array for storing values of t - Hyperion Years			//	no need for time here
		//time[0] = 0;			//inital time, *this cannot be modified
		
	//other initial conditions
	r[0] = 1; 					//initial radius - Hyperion unit(s)			
	x[0] = r[0];				//starting position is initial length of radius on the x axis - Hyperion Unit(s)
	y[0] = 0;					// ^
	vx[0] = 0;					//cannot be modified unless inital position is on the y-axis
	vy[0] = vy0i;					//Since we are starting on the x-axis, vy is our inital velocity
	theta[0] = q;				//Inital value of theta - radians
	angVelocity[0] = 0;			//inital angular velocity - radians / Hyperion Year

	//iteration
	int i = 0;
	while(i <= N){
		
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
		
		//theta reset //no need to bound theta. We want to measure its difference!
		//if(theta[i+1] > PI){
			//theta[i+1] -= 2*PI;
		//}
		//if(theta[i+1] < -PI){
			//theta[i+1] += 2*PI;
		//}
				
		//radius 
		r[i+1] = sqrt((x[i+1]*x[i+1]) + (y[i+1]*y[i+1]));
		//cout << r[i] << "\n";
		
			
		i++;
		
	}
	
	return theta[j];
	
}

int main()
{

	int N; 			//number of points 
	double T, dt; 	//time and step size
	dt = dti;		//Hyperion years	//defined by a constant for ease of access while varying initial conditions
	T = Ti;			//Hyperion years	//defined by a constant for ease of access while varying initial conditions
	N = T/dt;	 	//number of steps - not points
					
	double time[N+1];			//array for storing values of t - Hyperion Years		
		time[0] = 0;			//inital time, *this cannot be modified
		
	double PhaseDiff = 0.01;	//radians
	
	//iteration
	int i = 0;
	
	double theta1[N+1], theta2[N+1], dtheta[N+1];
		dtheta[0] = PhaseDiff;
		
	while(i <= N){

		theta1[i+1] = hyperion(0, i+1);
		theta2[i+1] = hyperion(PhaseDiff, i+1);

		//time array
		time[i+1] = (i+1)*dt;
		
		dtheta[i+1] = log(sqrt((theta1[i+1]-theta2[i+1])*(theta1[i+1]-theta2[i+1])));
		
		//no need to bound theta. We want to measure its difference!
		//if(dtheta[i+1] >= PI){
			//dtheta[i+1] -= 2*PI;
		//}
		//if(dtheta[i+1] <= -PI){
			//dtheta[i+1] += 2*PI;
		//}

		//cout << "\ni = " << i << ",\ntheta1 = " << theta1[i] << ",\ntheta2 = " << theta2[i] << ",\nDTHETA = " << dtheta[i];			

		i++;

	}
		
	//gnuplot_one_function ("Hyperion orbiting Saturn", "lines", "x", "y", x, y, N+1);
	//gnuplot_one_function ("Theta Vs. Time", "lines", "Time / Hyperion Years", "Theta", time, theta, N+1);
	//gnuplot_one_function ("Angular Velocity Vs. Time", "lines", "Time / Hyperion Years", "AngVelocity", time, angVelocity, N+1);
	gnuplot_one_function ("Change in Theta Vs. Time", "lines", "Time / Hyperion Years", "Change in Theta / radians", time, dtheta, N+1);	
	
	
	return 0;
}

