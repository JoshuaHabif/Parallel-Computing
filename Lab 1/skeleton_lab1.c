// must compile with -std=c99 -Wall -o checkdiv 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[]){
  
unsigned int a, b, c, n;
unsigned int i; //loop index
FILE * fp; //for creating the output file
char filename[100]=""; // the file name
char * x; //the numbers in the range [2, N]

clock_t start_p1, start_p3, end_p1,  end_p3;


/////////////////////////////////////////
// start of part 1

start_p1 = clock();
// Check that the input from the user is correct.
if(argc != 5){

  printf("usage:  ./checkdiv N a b c\n");
  printf("N: the upper bound of the range [2,N]\n");
  printf("a: first divisor\n");
  printf("b: second divisor\n");
  printf("c: third divisor\n");
  exit(1);
}  

n = (unsigned int)atoi(argv[1]); 
a = (unsigned int)atoi(argv[2]);
b = (unsigned int)atoi(argv[3]);
c = (unsigned int)atoi(argv[4]);  


// Process 0 must send the a, c, and n to each process.
// Other processes must, after receiving the variables, calculate their own range.


end_p1 = clock();
//end of part 1
/////////////////////////////////////////


/////////////////////////////////////////
//start of part 2
// The main computation part starts here
  
// end of the main compuation part
//end of part 2
/////////////////////////////////////////


/////////////////////////////////////////
//start of part 3
//forming the filename

start_p3 = clock();

strcpy(filename, argv[1]);
strcat(filename, ".txt");

if( !(fp = fopen(filename,"w+t")))
{
  printf("Cannot create file %s\n", filename);
  exit(1);
}

for(i = 2; i <= n; i++)
  if(x[i])
    fprintf(fp,"%d ", i);

fclose(fp);

end_p3 = clock();
//end of part 3
/////////////////////////////////////////

/* Print here the times of the three parts as indicated in the lab description */

return 0;
}

