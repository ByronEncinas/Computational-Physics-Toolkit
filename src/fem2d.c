#include <stdio.h>
#include "../include/fem2d.h"
#include <stdlib.h>
#include <math.h>

void B_mat(int size, float ks[size][3], float a[size][3], float b[size][3], float c[size][3], float B[size][size],float Kx_mat[size][size],float Ky_mat[size][size],float P_mat[size][size],int nodes[(size-2)*3]){
	
	float A[size-2];
	float div[size-2]; 	
	int i, j, k;
//		int nodes[6] = {1,3,4,2,1,4};

	for (i=0;i<size-2;i++){
		A[i] = (b[i][0]*c[i][1]-b[i][1]*c[i][0])/2;
		
		div[i] = 1/(4*A[i]);		
	}
		
	//Making the Kx matrix
	for(k=0;k<size-2;k++){
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
	

	for (i = 0; i < size; i++) {
    	for (j = 0; j < size; j++) {
        	B[i][j] += Kx_mat[i][j] + Ky_mat[i][j] + P_mat[i][j];
    	}
	}	 
}

void C_mat(int size,float* C, float a[size-2][3], float b[size-2][3], float c[size-2][3], float Q_val,int nodes[(size-2)*3]){
	
	float A[size-2];
	int i,j;

	for (j=0;j<size-2;j++){
		A[j] += (b[j][0]*c[j][1]-b[j][1]*c[j][0])/2;
	}
	for (j=0;j<size-2;j++){
		for(i=0;i<3;i++){
				C[nodes[3*j+i]-1] += (Q_val*A[j]/3);
		}
	}
}


void abcs_val(int size,float pointsx[size-2][3], float pointsy[size-2][3], float a[size-2][3], float b[size-2][3], float c[size-2][3]){
	
	int i;
	
	for (i=0;i<size-2;i++){
			
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

void BC(int size, float B[size][size], float* C, float bc[size-2][2]){
	int k;
	
	 for (k = 0; k<size; k++){
			      		
            B[k][(int)bc[0][0]-1] = B[k][(int)bc[0][0]-1]*bc[0][1];
            B[k][(int)bc[1][0]-1] = B[k][(int)bc[1][0]-1]*bc[1][1];

        }

        for (k = 0; k<size; k++){

                C[k] = C[k] - B[k][(int)bc[0][0]-1];
                C[k] = C[k] - B[k][(int)bc[1][0]-1];
                B[k][(int)bc[0][0]-1] = 0;
                B[k][(int)bc[1][0]-1] = 0;
        }

        B[(int)bc[0][0]-1][(int)bc[0][0]-1] = 1;
        B[(int)bc[1][0]-1][(int)bc[1][0]-1] = -1;
}


void zeros(int size, float matrix[size][size]){
	int i,j;
	for (i = 0; i < size; i++) {
    	for (j = 0; j < size; j++) {
        	matrix[i][j] = 0.0;
    	}
	}
}

void printing_assembly(int size, float B[size][size], float* C){
	int i,j;
	for (i=0;i<size;i++){
		for (j=0;j<size;j++){
			printf("%lf T_(%d,%d)\t",B[i][j],i,j);
		}
	printf("| %lf\n",C[i]);
		printf("\n");
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

void print_result(int size, float B[size][size], float* C){
	
	FILE *fpt;
	
	int i,j,k;
	
    // Create a file
    fpt = fopen("matrix.csv", "w+"); //Creating a csv file
    
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fprintf(fpt,"%lf,",B[i][j]); //Printing the values
        }
        printf("\n");
    }
	
	fpt = fopen("vector.csv", "w+"); //Creating a csv file
    for (k = 0; k<size; k++){
    	fprintf(fpt,"%lf,\n",C[k]);
	}	
}



