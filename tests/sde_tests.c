#include "../include/rk4.h"

int main(){

	// Defining parameters for the SDE
    int number,i, eqs;
    number = 7; // Number is equal to twice the number of coupled equations.
///////////////////////////////////////////////////////////////////////////////////////////
//Define the constants for the Initial Value Problem

//Resonator or cavity's parameters

//Atomic units
	float hbar = 1, e=1;
	
//	Proportionality constants

 	float C0 = 0.1; //Coupling parameter
 	float U = hbar/4, phi0 = hbar/(2*e);
 	float Cj = 5; 
 	float Ec = (e*e)/(2*Cj), Ej = 500*Ec;

//	L1 = (phi0**2)/Ej //Harmonic oscillator parameters
	float L1 = (phi0*phi0)/Ej;
	float C1 = 1;
	
//	L2 = (phi0**2)/(Ej) //JJ parameters
	float L2 = L1;

	float C2  = C1;
	
	float Ct = C0*C1+C0*C2+C1*C2;
	
	float Cj_1 = Ct/(C0+C2);
	float Cj_2 = Ct/(C0+C1);
	
	float omega_JJ_cavity = 1/sqrt(L1*Cj_1);
	float omega_JJ_qubit = 1/sqrt(L1*Cj_1);
	
	float k = C0/Ct;
//	omega_JJ_qubit = phi0*np.sqrt(Ej_cavity/Cj_2)
	
	float time_JJ_cavity = 25;
	
	int energy=1;
	
	float x0 =sqrt(2*energy*hbar/(Cj_1*omega_JJ_cavity));

////////////////////////////////////////////////////////////////////////////////////////////////////
    float elements[number], initial[7] = {0,x0,0,0,0,0,0}, constants1[] = {k,Cj_1,Cj_2,L1,Ej, phi0}; // Initial, in here we stablish the Initial conditions for the problem
    float h = 0.0001;
    int steps = 1/h, time = 25;
	
    
    matrix_copy(elements,initial, number);
    
    solution(elements, steps*time, h, number, constants1);
	// solution(*matrix, total_time, h, number, *consts)
    return 0;   
    
}

