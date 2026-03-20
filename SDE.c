#include<stdio.h>
#include<math.h>

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


float RK(float h,float *matrix1, int eqs,int len){
	float k1,k2,k3,k4;
	
	float matrix2[6] = {0,0,0,0,0,0};
	k1 = h*function(matrix1,matrix2,len,eqs);
	
	float matrix3[6] = {0.5*h,0.5*k1,0.5*h,0.5*k1,0.5*h,0.5*k1};
	k2 = h*function(matrix1,matrix3,len, eqs);

	float matrix4[6] = {0.5*h,0.5*k2,0.5*h,0.5*k2,0.5*h,0.5*k2};
	k3 = h*function(matrix1,matrix4,len, eqs);

	float matrix5[6] = {h,k3,h,k3,h,k3};
	k4 = h*function(matrix1,matrix5,len, eqs);

	return (k1+2*k2+2*k3+k4)/6;	
}



int main(){

    int number,i, eqs;
    
    number = 6;
    
    float elements[number], initial[6] = {0,1,0,0,0,0};
    float h = 0.0001;
    int steps = 1/h, time = 100;
	
	int j;
	
	FILE *fpt;

    
    matrix_copy(elements,initial, number);
    
    // Create a file
    fpt = fopen("filename.csv", "w+");

        	
	for (j=0; j<=steps*time;j++){
		
//		printf("%lf,%lf,%lf\n",x_10,y_10,y_20);	
        if (j%2==0){
			fprintf(fpt,"%lf,%lf,%lf,%lf,%lf,%lf\n",elements[0],elements[1],elements[2],elements[3],elements[4],elements[5]);
		}

	    
//		printf("%0.3lf,%0.3lf,%0.3lf\n",elements[0],elements[1],elements[3]);
	
		elements[1] = elements[1]+RK(h, elements, 0,number);
		elements[0]	+= h;	

		elements[3] = elements[3]+RK(h, elements, 1,number);
		elements[2]	+= h;

		elements[5] = elements[5]+RK(h, elements, 2,number);
		elements[4]	+= h;		
		

//        }
    }

    return 0;
}
