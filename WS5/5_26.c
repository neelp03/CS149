/**
 * Description: Exercise 5.26 Coding input length from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 3, 2023
 * Creation date: March 3, 2023
 **/
 
#include <stdio.h>
#include <string.h>

#define MAX_CHARS 50


/**
 * This function counts the number of characters from 
 * input
 * Assumption: Input will not exceed 50 characters
 * Input parameters: none
 * Returns: an integer
 **/
int main(void) {

	char ch;
	int count = 0;
	
	while((ch=getchar()) != '\n'){
		if((ch != ' ')&&(ch != '.')&&(ch != '!')&&(ch != ',')){
			count++;
		}
	}
	
	printf("%d\n", count);
	return 0;
    
}




