#include <stdio.h>
#include <mpi.h>  // This is the header file for MPI functions
#include <stdlib.h>
#include <string.h>
#include <time.h>

int acontains(int val, int arr[]);
double maxdouble(double arr[], int size);
/*
  int main(int argc, char* argv[])

  The main program is the starting point for an MPI program.
  This one simply reports its own process number.

*/

int main(int argc, char * argv[]) {
  // First thing is to initialize the MPI interface.  Some arguments can
  // be passed through to the MPI interface, so the argument list is
  // sent by sending the argument list
  MPI_Init( & argc, & argv);

  // Determine the rank of the current process
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);

  // Determine the number of processes
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, & size);

  /****************************************/
  //variable decleration 
  clock_t start_p1, start_p3, end_p1, end_p3;
  double start_p2, end_p2;
  double maxp2;
  int data[5];
  double * l;
  int * x; //the numbers in the range [2, N]
  unsigned int a, b, c, n;
  unsigned int i; //loop index
  FILE * fp; //for creating the output file
  char filename[100] = ""; // the file name

  /****************************************/

  //printf("I am process %d out of %d\n", rank, size);

  /////////////////////////////////////////
  // start of part 1

  start_p1 = clock();

  if (rank == 0) {
    // Check that the input from the user is correct.
    if (argc != 5) {

      printf("usage:  ./checkdiv N a b c\n");
      printf("N: the upper bound of the range [2,N]\n");
      printf("a: first divisor\n");
      printf("b: second divisor\n");
      printf("c: third divisor\n");
      exit(1);
    }

    n = (unsigned int) atoi(argv[1]);
    a = (unsigned int) atoi(argv[2]);
    b = (unsigned int) atoi(argv[3]);
    c = (unsigned int) atoi(argv[4]);
    data[0] = a;
    data[1] = b;
    data[2] = c;
    //compute and store the problem size
    data[3] = n / (size - 1);
    x = malloc(n * sizeof(int) + 1);
    l = malloc((data[3]+1) * sizeof(double) + 1);

    // Process 0 must send the a, c, and n to each process.
    // Other processes must, after receiving the variables, calculate their own range.
  }

  end_p1 = clock();
  double p1 = (end_p1 - start_p1) / CLOCKS_PER_SEC;
  //end of part 1
  /////////////////////////////////////////

  /////////////////////////////////////////
  //start of part 2
  // The main computation part starts here
  // If not the first process, wait for permission to proceed
  start_p2 = MPI_Wtime();
  if (rank > 0) {
    // Wait for a message from rank 0 process
    MPI_Recv(data, 4, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
      MPI_STATUS_IGNORE);
    /*
    printf("Rank %d has received message with data {%d, %d, %d, %d} from rank %d.\n",
	   rank, data[0], data[1], data[2], data[3], 0);*/

    int end = (rank) * data[3];
    //printf("rank: %d; start: %d; end: %d\n", rank, end-data[3], end);
    //the main computation happens here

    l = malloc((data[3]+1) * sizeof(double) + 1);
    //printf("Rank %d starts at %d and ends at %d\n", rank, end-data[3]+1, end);
    int counter = 0;
    for (int i = end - data[3] + 1; i <= end; i++) {
      if ((i % data[0] == 0) || (i % data[1] == 0) || (i % data[2] == 0)) {
        l[++counter] = i;
        //printf("Rank %d determined that %d should be added\n", rank, l[counter-1]);
      }
    }
    end_p2 = MPI_Wtime();
  	double p2 = (end_p2 - start_p2);
  	l[data[3]] = p2;
  	//printf("Rank %d p2: %f\n ", rank, l[data[3]]);
  }


  

  // end of the main compuation part
  //end of part 2
  /////////////////////////////////////////

  /////////////////////////////////////////
  //start of part 3
  //forming the filename

  start_p3 = clock();

  // Process 0 sends data to all processes but itself
  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(data, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    int counter = 0;
    double *doubles = malloc((size-1) * sizeof(double) + 1);
    //printf("size of %d", (size-1));
    // Wait for a message from rank > 0 process
    for (int i = 1; i < size; i++) {
      MPI_Recv(l, data[3] + 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
      doubles[i-1] = l[data[3]];

      for (int j = 0; j < data[3] + 1; j++) {
        if (l[j] != 0 && acontains(l[j], x) == 0)
          x[++counter] = l[j];
      	//free(l);
      }
    }

    maxp2 = maxdouble(doubles, size-1);
    //free(doubles);
  }

  if (rank > 0) {
    MPI_Send(l, data[3] + 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  
  if (rank == 0) {
    strcpy(filename, argv[1]);
    strcat(filename, ".txt");

    if (!(fp = fopen(filename, "w+t"))) {
      printf("Cannot create file %s\n", filename);
      exit(1);
    }

    for (i = 0; i <= n; i++)
      if (x[i] >= 2) {
        fprintf(fp, "%d\n", x[i]);
        //printf("%d\n", x[i]);
      }

    fclose(fp);
    //free(x);
    //free(data);

    end_p3 = clock();
    double p3 = (end_p3 - start_p3) / CLOCKS_PER_SEC;
    //end of part 3
    /////////////////////////////////////////

    // Print here the times of the three parts as indicated in the lab description 
    printf("times of part1 = %f s, part2 = %f s, part3 = %f s\n", p1, maxp2, p3);
  }
  

  // Must shut down the MPI system when you're done.  
  // If you don't, there can be lots of junk left behind.
  MPI_Finalize();
  return 0;
}

int acontains(int val, int arr[]) {
  int i;
  for (i = 0; i < sizeof(arr); i++) {
    if (arr[i] == val)
      return 1;
  }
  return 0;
}

double maxdouble(double arr[], int size) {
  int i;
  double max = arr[0];
  //printf("arr size: %d\n", sizeof(arr));
  for (i = 1; i < size; i++) {
  	//printf("%d\n", i);
    if (arr[i-1] < arr[i])
      max = arr[i];
  }
  return max;
}