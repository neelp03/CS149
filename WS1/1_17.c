/**
 * Description: Exercise 1.17 from Zybooks
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: 01/31/2023
 * Creation date: 01/31/2023
 **/


#include <stdio.h>
#include <stdlib.h>

/**
 * This function prints a right-facing arrow
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
**/
int main(void) {
	int height = 3, width = 5;
	int baseInt, headInt;
	// stores user input into baseInt and headInt
	scanf("%d", &baseInt);
	scanf("%d", &headInt);
	
	// for loop that starts by printing the base when its at the right position/height
	for(int i = -height; i<=height; i++) {
		for(int j = 0; j < width; j++) {
			if(abs(i) > height / 2) {
				printf("%c",' ');
			} else {
				printf("%d", baseInt);
			}
		}
		// prints the pointer of the arrow
		for(int j = 0; j < height - abs(i) + 1; j++) {
			printf("%d", headInt);
		}
		printf("\n");
	}
	return 0;
}

