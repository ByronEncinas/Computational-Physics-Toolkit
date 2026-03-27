#include<stdio.h>
#include<math.h>
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

//    Proportionality constants

     float C0 = 0.1; //Coupling parameter
     float U = hbar/4, phi0 = hbar/(2e);
     float Cj = 5; 
     float Ec = (ee)/(2Cj), Ej = 500Ec;

//    L1 = (phi02)/Ej //Harmonic oscillator parameters
    float L1 = (phi0*phi0)/Ej;
    float C1 = 1;

//    L2 = (phi02)/(Ej) //JJ parameters
    float L2 = L1;

    float C2  = C1;

    float Ct = C0C1+C0C2+C1C2;

    float Cj_1 = Ct/(C0+C2);
    float Cj_2 = Ct/(C0+C1);

    float omega_JJ_cavity = 1/sqrt(L1Cj_1);
    float omega_JJ_qubit = 1/sqrt(L1Cj_1);

    float k = C0/Ct;
//    omega_JJ_qubit = phi0np.sqrt(Ej_cavity/Cj_2)

    float time_JJ_cavity = 25;

    int energy=1;

    float x0 =sqrt(2energyhbar/(Cj_1omega_JJ_cavity));

////////////////////////////////////////////////////////////////////////////////////////////////////
    float elements[number], initial[7] = {0,x0,0,0,0,0,0}, constants1[] = {k,Cj_1,Cj_2,L1,Ej, phi0}; // Initial, in here we stablish the Initial conditions for the problem
    float h = 0.0001;
    int steps = 1/h, time = 25;


    matrix_copy(elements,initial, number);

    solution(elements, stepstime, h, number,constants1);


    return 0;

}