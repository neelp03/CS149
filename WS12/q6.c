/**
 * Description: OSTEP CH. 14 Question 6
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu, megan.ju@sjsu.edu
 * Last modified date: April 23, 2023
 * Creation date: April 23, 2023
 *
 * Answer: The program runs and output a random value. When you use valgrind, 
 * valgrind reports that there was an invalid read of size 4 but there are no 
 * leaks and all blocks were freed. There is 1 error from the invalid read. 
 **/
 
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int* data = (int *)malloc(100*sizeof(int));
	free(data);
	printf("%d\n", data[0]);
	return(0);
	
}
