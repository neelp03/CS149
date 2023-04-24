/**
 * Description: OSTEP CH. 14 Question 5
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu, megan.ju@sjsu.edu
 * Last modified date: April 23, 2023
 * Creation date: April 23, 2023
 *
 * Answer: The program executes normally when it runs. When running the program 
 * with valgrind, it says that there are 2 errors from 2 contexts. There is an 
 * invalid write of size 4 and 400 bytes are definitely lost. Valgrind is 
 * correct.
 **/
 
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int* data = (int *)malloc(100*sizeof(int));
	data[100] = 0;
	return(0);
	
}
