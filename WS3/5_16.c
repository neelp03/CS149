/**
 * Description: Exercise 5.16 - Output numbers in reverse from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 13, 2023
 * Creation date: February 13, 2023
 **/
 
#include <stdio.h>

#define MAX_LENGTH 19

/**
 * This function prints out an integer in reverse binary form
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	
 	// Obtain the amount of numbers in the list
 	int length;
 	scanf("%d", &length);
 	
 	// If the length is longer than requirement,
 	// return 1
 	if(length > MAX_LENGTH){
 		return 1;
 	}
 
 	// Create array for output
 	int list[length];
 	int temp = 0;	// Temporary variable to hold values from list
 	
 	// Put the numbers in the list into an array
 	for(int i = 0; i < length; i++){
 		scanf("%d", &temp);
 		list[i] = temp;
 	}
 	
 	// Print out the array in reverse
 	for(int j = length-1; j >= 0; j--){
 		printf("%d,", list[j]);
 	}
 	
 	printf("\n");
 	return 0;	// Successful execution
 	
 }
