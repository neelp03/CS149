/**
 * Description: Exercise 2.34 from Zybooks
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: February 8, 2023
 * Creation date: February 8, 2023
 **/

#include <stdio.h>
#include <math.h>

/**
 * This function outputs a welcome message with the input
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
int main(void) {
	char name[30];

	// get user input
	scanf("%s", name);
	
	//print welcome message
	printf("Hello %s, and welcome to CS Online!\n", name);
	
	return 0;

}
