#include<stdio.h>
#include<math.h>

//This function takes a given matrix, and returns another one. 
//The first matrix in this code is the one where the initial conditions values are going to be stored.
float matrix_copy(float *matrix1,float *matrix2, int len){
	int i;
	
	for(i=0;i<=len;i++){
		
		matrix1[i] = matrix2[i];
	}
	
	return *matrix1;
}

//This function is for defining the system of diferential equations (SDE) that we want to solve
float function(float *matrix1, float *matrix2, int len, int eqs, float *consts){
	int i;
	float place_holder[len/2]; //As the name indicates, this will be just a placeholder, as the funcions may change their structure depending on the SDE.
							   //This place holder will allow us to store in a single C function the whole SDE
	float hold[len]; //Hold defines the variables we are worunge_kutta_o4ing with, that is x_0,y_0, x_1,y_1,..., x_n,y_n
	
	for(i=0;i<=len;i++){
		
		hold[i] = matrix1[i] + matrix2[i]; //This implement the changes required in the runge_kutta_o44 method, matrix1 is the initial value matrix, whereas matrix2
										   //Will be updated throughout the whole evaluation
	}
	
	//Write the equations of motion for n-pairs of equations
	//They are of the form x_0,y_0,x_1,y_1,...,x_n-1,y_n-1	

	if (eqs == 0){
		place_holder[0] = hold[3]/consts[1]+consts[0]*hold[7];  
	}
	else if(eqs == 1){
		place_holder[1] = -hold[1]/consts[3];
	}
	else if(eqs == 2){
		place_holder[2] = hold[7]/consts[2]+consts[0]*hold[3];
	}
	else if(eqs == 3){
		place_holder[3] = -(consts[4]/consts[5])*(hold[5]/consts[5]);
	}


	return place_holder[eqs]; //This will return the value of a given equation
}


//This function will allow us to implement the runge_kutta_o4 to n number of variables
float matrix_var(float *matrix, float var1, float var2, int len){
	
	int i;
	
	for (i=0;i<=len/2;i++){
		
		matrix[2*i] = var1;   //This will copy the values var1 for the x's 
		matrix[2*i+1] = var2; //This will copy the values var2 for the y's			
	}
	
	return *matrix;
	
}


//This function is the core of the runge_kutta_o44 method, it will call all the previous one to accomplish it.
// h is the stepsize, matrix1 is the initial matrix, eqs will help us select among all the equations in the SDE, and finally consts
// are the constants values that multiply the variables
float runge_kutta_o4(float h,float *matrix1, int eqs,int len, float *consts){
	float k1,k2,k3,k4;
	
	float matrix2[len];
	matrix_var(matrix2, 0,0,len); //This will take the values 0,0 and create an array -> 0,0,0,0,...,0,0
	k1 = h*function(matrix1,matrix2,len,eqs,consts);
	
	float matrix3[len]; 
	matrix_var(matrix2,0.5*h,0.5*k1,len);//This will take the values 0.5h,0.5k1 and create an array -> 0.5h,0.5k1,0.5h,0.5k1,...,0.5h,0.5k1
	k2 = h*function(matrix1,matrix3,len, eqs,consts);
	
	float matrix4[len];
	matrix_var(matrix2,0.5*h,0.5*k2,len);//This will take the values 0.5h,0.5k2 and create an array -> 0.5h,0.5k2,0.5h,0.5k2,...,0.5h,0.5k2
	k3 = h*function(matrix1,matrix4,len, eqs,consts);
	
	float matrix5[len];
	matrix_var(matrix2, h,k3,len);//This will take the values h,k3 and create an array-> h,k3,h,k3,...,h,k3
	k4 = h*function(matrix1,matrix5,len, eqs,consts);
	
	return (k1+2*k2+2*k3+k4)/6;	
}



//This part of the code it is just the iteration of the previous functions, and final creation of a csv file with the results
void solution(float *matrix, float total_time, float h, float number, float *consts){
	
	FILE *fpt;
	
    // Create a file
    fpt = fopen("filename.csv", "w+"); //Creating a csv file
    
    int j,k;

        	
	for (j=0; j<=total_time;j++){
		
	//		printf("%lf,%lf,%lf\n",x_10,y_10,y_20);	
        if (j%2==0){
			fprintf(fpt,"%lf,%lf,%lf,%lf\n",matrix[0],matrix[1],matrix[3],matrix[5]); //Printing the values

		}

	    //		printf("%0.3lf,%0.3lf,%0.3lf\n",elements[0],elements[1],elements[3]);
	//Running the runge_kutta_o4 algorithm for all the variables in the SDE
		for (k=0;k<=number/2;k++){
				matrix[2*k+1] = matrix[2*k+1]+runge_kutta_o4(h, matrix, k,number, consts); 
				matrix[2*k]	+= h;	
		}
    }
}

