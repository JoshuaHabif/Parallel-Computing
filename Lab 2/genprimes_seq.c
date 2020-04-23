//compile using gcc -g -Wall -fopenmp -std=c99 -o genprime genprimes.c -lm


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]){

	/* N is a positive number bigger than 2 and less than or 
	 equal to 100,000,000.
	 t is the number of threads and is a positive integer 
	 that does not exceed 100.*/
	unsigned int N, t, end;


	/*PART 1: Read the input from the command line*/

	//check that the input is correct
	if (argc != 3){
	  printf("usage: ./genprime N t\n");
		exit(1);
	}

	N = (unsigned int) atoi(argv[1]);
	t = (unsigned int) atoi(argv[2]);

	FILE * fp; //for creating the output file
	//why 100?????
	char filename[100]=""; // the file name
	int * x; //the numbers in the range [2,N]
	end = floor((N+1)/2);
	double tstart = 0.0, ttaken;
	//DIAGNOSITC
	//printf("\nend: %d\n",end);


	/*PART 2: Generate the prime numbers*/

	tstart = omp_get_wtime();

	//Algorithm for generating prime numbers:
	//Step 1: generate all numbers from 2 to N.
	x = malloc(N * sizeof(int)-2);
	for(int i = 0; i<N-1; i++){
		x[i] = i+2;
	}

	/*Step 2-5: first number is 2, so remove all 
	 numbers that are multiple of 2 (i.e. 4, 6, 8, ...).
	 Do not remove the 2 itself.*/
	for(int k=0; k<N-1; k++){
		for(int i=k+1; i<N-1; i++){
			//DIAGNOSITC
			//printf("k=%d \t i=%d \t x[i] mod x[k] = %d mod %d = %d\n", k, i, x[i], x[k], x[i]%x[k]);
			if((x[i]%x[k]==0) && (x[i]!=-1) && (x[k]!=-1))
				x[i] = -1;
			//DIAGNOSITC
			//printf("x[%d]=%d\n", i, x[i]);
		}	
	}

	ttaken = omp_get_wtime() - tstart;
	printf("Time taken for the main part: %f\n", ttaken);

	//DIAGNOSITC
	/*
	printf("\n**********************************\n");
	//check
	for(int i=0; i<N-1; i++){
		//printf("the %dth number is %d\n", i ,x[i]);
		if(x[i]!=-1)
			printf("%d, ", x[i]);
	}
	printf("\n**********************************\n");
	*/


	/*PART 3: Write the output file and exit*/

	//forming the filename
	strcpy(filename, argv[1]);
	strcat(filename, ".txt");

	if(!(fp = fopen(filename, "w+t"))){
		printf("Cannot create file %s\n", filename);
		exit(1);
	}

	for(int i=0; i<N-1; i++)
		if(x[i]!=-1)
			fprintf(fp, "%d\n", x[i]);

	fclose(fp);
	free(x);

	return 0;

}
