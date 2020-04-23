//Login to one of the crackle servers and type: module load gcc-9.2
//compile using gcc -g -Wall -fopenmp -std=c99 -o genprime genprimes.c -lm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h> // This is the header file for OpenMP directives

int main(int argc, char *argv[]){

	/* N is a positive number bigger than 2 and less than or 
	 equal to 100,000,000.
	 t is the number of threads and is a positive integer 
	 that does not exceed 100.*/
	unsigned int N, t, end;

	bool WRITE_OUTPUT = true;
	bool DIAGNOSITC = false;


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
	end = sqrt(N+1);
	double tstart, tend, ttaken;

	//DIAGNOSITC
	if(DIAGNOSITC)
		printf("\nend: %d\n",end);


	/*PART 2: Generate the prime numbers*/

	tstart = omp_get_wtime();

	//Algorithm for generating prime numbers:

	/*Step 1: generate all numbers from 2 to N.
	we do this implictly, treating the index + 2 as the number.*/
	x = malloc(N * sizeof(int)-2);

	/*Step 2-5: first number is 2, so remove all 
	 numbers that are multiple of 2 (i.e. 4, 6, 8, ...).
	 Do not remove the 2 itself.*/
	int k, i;
	#pragma omp parallel for num_threads(t) default(none) private(k, i) shared (x, end, N)
	for(k=0; k<end; k++){	
		for(i=k+1; i<N-1; i++){
			if((x[i]!=-1) && (x[k]!=-1) && ((i+2)%(k+2)==0))
				x[i] = -1;
		}		
	}
	
	
	tend = omp_get_wtime();
	ttaken = tend - tstart;
	printf("Time taken for the main part: %f\n", ttaken);

	//DIAGNOSITC
	if(DIAGNOSITC){
		printf("\n**********************************\n");
		for(int i=0; i<N-1; i++){
			if(x[i]!=-1)
				printf("%d, ", i+2);
		}
		printf("\n**********************************\n");
	}


	/*PART 3: Write the output file and exit*/

	//forming the filename
	strcpy(filename, argv[1]);
	strcat(filename, ".txt");

	if(!(fp = fopen(filename, "w+t"))){
		printf("Cannot create file %s\n", filename);
		exit(1);
	}

	if(WRITE_OUTPUT){
		for(int i=0; i<N-1; i++)
			if(x[i]!=-1)
				fprintf(fp, "%d\n", (i+2));
	}
	fclose(fp);
	free(x);

	return 0;

}
