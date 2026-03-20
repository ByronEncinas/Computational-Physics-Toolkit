#include<stdio.h>
#include<math.h>

float matrix_copy(float *matrix1,float *matrix2, int len);

float matrix_var(float *matrix,float var1, float var2, int len);

float function(float *matrix1, float *matrix2, int len, int eqs);

float RK(float h,float *matrix1, int eqs,int len);

void solution(float *matrix, float total_time, float h, float number);

int main(){

    int number,i, eqs;
    
    number = 6;
    
    float elements[number], initial[6] = {0,1,0,0,0,0};
    float h = 0.001;
    int steps = 1/h, time = 100;
	
    
    matrix_copy(elements,initial, number);
    
    solution(elements, steps*time, h, number);
    
    
    return 0;
}

float matrix_copy(float *matrix1,float *matrix2, int len){
	int i;
	
	for(i=0;i<=len;i++){
		
		matrix1[i] = matrix2[i];
	}
	
	return *matrix1;
}


float function(float *matrix1, float *matrix2, int len, int eqs){
	int i;
	float place_holder[len/2];
	float hold[len];
	
	for(i=0;i<=len;i++){
		
		hold[i] = matrix1[i] + matrix2[i];
	}
	
//Write the equations of motion for n-pairs of equations
//They are of the form x_0,y_0,x_1,y_1,...,x_n-1,y_n-1	

	if (eqs == 0){
		place_holder[0] = 10*(hold[3]-hold[1]);
	}
	else if(eqs == 1){
		place_holder[1] = 28*hold[1]-hold[3]-hold[1]*hold[5];
	}
	else if(eqs == 2){
		place_holder[2] = hold[1]*hold[3]-8*hold[5]/3;
	}

	return place_holder[eqs];
}



float matrix_var(float *matrix, float var1, float var2, int len){
	
	int i;
	
	for (i=0;i<=len/2;i++){
		
		matrix[2*i] = var1;
		matrix[2*i+1] = var2;			
	}
	
	return *matrix;
	
}



float RK(float h,float *matrix1, int eqs,int len){
	float k1,k2,k3,k4;
	
	float matrix2[len];
	matrix_var(matrix2, 0,0,len);
	k1 = h*function(matrix1,matrix2,len,eqs);
	
	float matrix3[len];
	matrix_var(matrix2,0.5*h,0.5*k1,len);
	k2 = h*function(matrix1,matrix3,len, eqs);
	
	float matrix4[len];
	matrix_var(matrix2,0.5*h,0.5*k2,len);
	k3 = h*function(matrix1,matrix4,len, eqs);
	
	float matrix5[len];
	matrix_var(matrix2, h,k3,len);
	k4 = h*function(matrix1,matrix5,len, eqs);
	

	return (k1+2*k2+2*k3+k4)/6;	
}

void solution(float *matrix, float total_time, float h, float number){
	
	FILE *fpt;
	
    // Create a file
    fpt = fopen("filename.csv", "w+");
    
    int j,k;
	
	for (j=0; j<=total_time;j++){
		
//		printf("%lf,%lf,%lf\n",x_10,y_10,y_20);	
        if (j%2==0){
			fprintf(fpt,"%lf,%lf,%lf,%lf,%lf,%lf\n",matrix[0],matrix[1],matrix[2],matrix[3],matrix[4],matrix[5]);
		}

	    
//		printf("%0.3lf,%0.3lf,%0.3lf\n",elements[0],elements[1],elements[3]);
		for (k=0;k<=number/2;k++){
				matrix[2*k+1] = matrix[2*k+1]+RK(h, matrix, k,number);
				matrix[2*k]	+= h;	
		}
		

//        }
    }
	return 0;
}
