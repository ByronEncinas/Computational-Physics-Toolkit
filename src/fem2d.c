#include <stdio.h>
#include "../include/fem2d.h"
#include <stdlib.h>
#include <math.h>

void B_mat(float ks[2][3], float a[2][3], float b[2][3], float c[2][3], float B[4][4],int nodes[6]){
	
	float A[2] = {(b[0][0]*c[0][1]-b[0][1]*c[0][0])/2,(b[1][0]*c[1][1]-b[1][1]*c[1][0])/2};
	float div[2] = {1/(4*A[0]),1/(4*A[1])};
	float Kx_mat[4][4], Ky_mat[4][4], P_mat[4][4];
	int i, j, k;
//		int nodes[6] = {1,3,4,2,1,4};

		
	//Making the Kx matrix
	for(k=0;k<2;k++){
		for (i=0;i<3;i++){
			for (j=0;j<3;j++){
			
				Kx_mat[nodes[3*k+i]-1][nodes[3*k+j]-1] += (ks[k][0]*div[k])*b[k][i]*b[k][j];
				Ky_mat[nodes[3*k+i]-1][nodes[3*k+j]-1] += (ks[k][1]*div[k])*c[k][i]*c[k][j];
				
				if (i==j){
					P_mat[nodes[3*k+i]-1][nodes[3*k+j]-1]  += -(ks[k][2]*A[k]/6);
				} else {
					
					P_mat[nodes[3*k+i]-1][nodes[3*k+j]-1]  += -(ks[k][2]*A[k])/12;
				}
				
			}	
			
		}
	}
	
	
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
		
		B[i][j] = Kx_mat[i][j]+Ky_mat[i][j]+P_mat[i][j];		
	//	printf("Kx (%d,%d) = %lf, Ky (%d,%d)= %lf, P (%d,%d)= %lf\n",i+1,j+1, Kx_mat[i][j],i+1,j+1,Ky_mat[i][j],i+1,j+1, P_mat[i][j]);			
//		printf("B(%d,%d) = %.3lf\t",i,j,B[i][j]);
		}
		
//		printf("\n");
	}	 
}

void C_mat(float* C, float a[2][3], float b[2][3], float c[2][3], float Q_val,int nodes[6]){
	
	float A[2] = {(b[0][0]*c[0][1]-b[0][1]*c[0][0])/2,(b[1][0]*c[1][1]-b[1][1]*c[1][0])/2};
	int i,j;
	
	for (j=0;j<2;j++){
		for(i=0;i<3;i++){
				C[nodes[3*j+i]-1] += (Q_val*A[j]/3);
		}
	}
}


void abcs_val(float pointsx[2][3], float pointsy[2][3], float a[2][3], float b[2][3], float c[2][3]){
	
	int i;
	
	for (i=0;i<2;i++){
			
		a[i][0] =  pointsx[i][1]*pointsy[i][2]-pointsx[i][2]*pointsy[i][1];
		b[i][0] =  pointsy[i][1]-pointsy[i][2];
		c[i][0] =  pointsx[i][2]-pointsx[i][1];
	
		a[i][1] =  pointsx[i][2]*pointsy[i][0]-pointsx[i][0]*pointsy[i][2];
		b[i][1] =  pointsy[i][2]-pointsy[i][0];
		c[i][1] =  pointsx[i][0]-pointsx[i][2];
		
		a[i][2] =  pointsx[i][0]*pointsy[i][1]-pointsx[i][1]*pointsy[i][0];
		b[i][2] =  pointsy[i][0]-pointsy[i][1];
		c[i][2] =  pointsx[i][1]-pointsx[i][0];
	}
}


void printing_points(float* pointsx, float* pointsy, int num_triang){
	
	int i,k;
	
	FILE *fpt;
	fpt = fopen("FEM2D_points.csv", "w+");
	

	for(i=0;i<num_triang*3;i++){
		fprintf(fpt,"%0.8lf,%0.8lf\n", pointsx[i], pointsy[i]); //Saving the values
		printf("%0.8lf,%0.8lf\n", pointsx[i], pointsy[i]); //Printing the values
	}
		
}

void print_result(float B[4][4], float* C){
	
	FILE *fpt;
	
	int i,j,k;
	
    // Create a file
    fpt = fopen("matrix.csv", "w+"); //Creating a csv file
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            fprintf(fpt,"%lf,",B[i][j]); //Printing the values
        }
        printf("\n");
    }
	
	fpt = fopen("vector.csv", "w+"); //Creating a csv file
    for (k = 0; k<4; k++){
    	fprintf(fpt,"%lf,\n",C[k]);
	}	
}



