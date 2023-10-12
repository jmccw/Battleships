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
#include <fstream>
#include <sstream> 

using namespace std; 

const double PI = 3.141592653589;

//consistently changed variables - for ease of access.
const double Ti = 10;
const double dti = 0.0005;
const double vy0i = 6;
const double PhaseDiff_i = 0.01;

double hyperion(double q, int j){	//copied from main in previous program, only what needs to be run. did this to save memory - but not time unfortunately
									//q is the initial value for theta, j is the iteration which is called at the end - unfortuantely the only viable way to do this. -Too many arrays.
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
		//no need to bound theta. We want to measure its difference now.
				
		//radius 
		r[i+1] = sqrt((x[i+1]*x[i+1]) + (y[i+1]*y[i+1]));	
		
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
	
	//for Lyapunov Analysis:
	double e;		//ecentricty
	double a;		//semi-major axis	
	
			//ecentricity model//
		//while:
		
		//vy0 < 2PI; vy0 = v_min
		//		a = 1/1+e; 		e = 1-(v/2PI)^2
		
		//vy0 > 2PI; vy0 = v_max
		//		a = 1/1-e; 		e = (v/2PI)^2 - 1
		
	//thus:
	if(vy0i < 2*PI){
		e = 1 -((vy0i/(2*PI))*(vy0i/(2*PI)));	//eccentricity of the system
		a = 1/(1+e);
	}else if(vy0i > 2*PI){
		e = (vy0i/(2*PI))*(vy0i/(2*PI)) - 1;	//eccentricity of the system
		a = 1/(1-e);
	}
	else if(vy0i == 2*PI){	//Circular orbit
		e = 0;				//eccentricity of the system
		a = 1;
	}
					
	double time[N+1];				//array for storing values of t - Hyperion Years		
		time[0] = 0;				//inital time, *this cannot be modified
		
	double PhaseDiff = PhaseDiff_i;	//radians
	
	//iteration
	double theta1[N+1], theta2[N+1], dtheta[N+1];
		dtheta[0] = PhaseDiff;
			double dtheta_actual[N+1];	//holds actual value for storing in file, see below, dtheta holds logarithmic for gnuplot
	
	//arrays to store values used in calculating the lyapunov	//these were set to 250 - as this left room for easy manipulating without 	
	double Lyapunov[1000];										//without taking up too much memory - a minimal amount of waste where possible
	double LyapunovGRAD[1000];
	
	int i = 0;
	int j = 0;	
	double w[1000];
	while(i <= N){

		theta1[i+1] = hyperion(0, i+1);
		theta2[i+1] = hyperion(PhaseDiff, i+1);

		//time array
		time[i+1] = (i+1)*dt;
		
		//assigning dtheta into the array
		dtheta[i+1] = log(sqrt((theta1[i+1]-theta2[i+1])*(theta1[i+1]-theta2[i+1])));	//specifically for gnuplot (logarithmic axes)
		dtheta_actual[i+1] = sqrt((theta1[i+1]-theta2[i+1])*(theta1[i+1]-theta2[i+1]));	//no log, raw value - this was used in previous versions to store data in a file for analysis elsewhere
		
		//% is the modulus operator, it finds the remainder if divided by a specifed number - here it is used to ensure the code is run only every 100 iterations.																		
		if(i%8000 == 0){
			
			w[j] = time[i+1];
			Lyapunov[j] = dtheta[i+1];		//we take the logarithmic value as the Lyapunov is an exponential exponent
			
			//this does not calculate on initial iteration, do not want values of 0 when delaing with Lyapunov as this may have a critical effect on result
			if(j > 0){
				LyapunovGRAD[j] = (Lyapunov[j]-Lyapunov[j-1])/(w[j]-w[j-1]);
				//cout << "\nLyapunovGRAD: " << LyapunovGRAD[j];
			}
			//cout << "\nLyapunov: " << Lyapunov[j];
			j++;
		}		
		
		i++;
	}
	
	//Calculating Lyapunov approximation for qualitative analysis
	double LyapunovGRADsum = 0; 
	for(int w = 0; w <= 100; w++){
		LyapunovGRADsum += LyapunovGRAD[w];
	}
	double LyapunovGRADfinal = LyapunovGRADsum/j;

	cout << "\n\te = " << e;
	cout << "\n\ta = " << a;
	//cout << "\n\tj (Lyupanov iterations) = " << j << "\n";
	cout << "\n\tLyapunov = " << LyapunovGRADfinal << "\n\n";
	
	gnuplot_one_function ("Change in Theta Vs. Time", "lines", "Time / Hyperion Years", "Change in Theta / radians", time, dtheta, N+1);		
	gnuplot_one_function ("Change in Theta Vs. Time", "lines", "Time / Hyperion Years", "Change in Theta / radians", time, dtheta_actual, N+1);		
	
	
	return 0;
}
