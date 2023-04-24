/**
 * Description: OSTEP CH. 14 Question 4
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu, megan.ju@sjsu.edu
 * Last modified date: April 23, 2023
 * Creation date: April 23, 2023
 *
 * Answer: The program executes normally. Since there is no free after allocating
 * memory, there is a memory leak. gdb doesn't report any problems and stays that 
 * the process exited normally. Valgrind reports thta there was a memory lead of 
 * 4 bytes in one block that was definitely lost at a specific address. 
 **/
 
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int *ptr;
	ptr = (int *)malloc(4);
	*ptr = 10;
	return(0);
	
}
