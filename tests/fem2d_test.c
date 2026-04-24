#include <stdio.h>
#include "../include/fem2d.h"
#include <stdlib.h>
#include <math.h>


//In this test, we  workout the example 10.1 shown in
// Basics of the finite element method by Paul E. Allaire

int main(){
	
	//Defining mesh triangular parameters
	int num_t = 2, i,j,k;
	int num_nodes = num_t+2;
	
	float pointsx[2][3] = {{0,160,110},{50,0,110}};
	float pointsy[2][3] = {{0,0,120},{120,0,120}};
	int nodes[6] = {1,3,4,2,1,4};
	float bc[2][2] = {{1,330},{3,250}};
	
	
	//Defining the a,b,c's constants for the nodes
	float a[num_t][3], b[num_t][3], c[num_t][3];	
	abcs_val(num_nodes,pointsx, pointsy, a, b, c);
	
	//printf("%lf %lf %lf", a[0][0],b[0][0], c[0][0]);

	
	//Defining the B matrix
	float B[4][4], Kx[4][4], Ky[4][4], P[4][4];
	float ks[2][3]={{0.25,0.25,-2*pow(10,-5)},{0.25,0.25,-2*pow(10,-5)}};
	
	zeros(num_nodes,B);
	zeros(num_nodes,Kx);
	zeros(num_nodes,Ky);
	zeros(num_nodes,P);	
		
	B_mat(num_nodes, ks, a, b, c, B, Kx,Ky,P,nodes);

	
	//Defining C matrix
	float C[num_nodes], Q_val = 6*pow(10,-4);
	C_mat(num_nodes,C, a, b, c, Q_val,nodes);
	
	
	//Applying the BC
	BC(num_nodes, B, C, bc);
		
	//Printing on the screen
	printing_assembly(num_nodes,B, C);
	
	//Exporting the result
	print_result(num_nodes,B, C);
		

	 	
	return 0;
	
}

