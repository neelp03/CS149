/**
 * Description: Exercise 5.17 - Middle item from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 13, 2023
 * Creation date: February 13, 2023
 **/
 
#include <stdio.h>

#define MAX_LENGTH 9

/**
 * This function prints out an integer in reverse binary form
 * Assumption: Number of integers is always odd
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 
 	int list[MAX_LENGTH];	// Array to hold numbers
 	int tempNum = 0;	// Temp var to place numbers in array
 	int i = 0;	// Indexing variable
 	int midNum = 0;	// Middle number's index
 	
 	// Iterate through the given numbers
 	while(i < MAX_LENGTH){
 	
 		scanf("%d", &tempNum);
 		
 		// If there are too many numbers, error and unsuccesful
 		if(tempNum > 0 && i == MAX_LENGTH){
 			printf("Too many numbers\n");
 			return 1;
 		}
 		
 		// If run into negative number, end loop
 		if(tempNum < 0 && i < MAX_LENGTH){
 			break;
 		}
 		// Add number to array
 		else {
 			list[i] = tempNum;
 		}
 		
 		i++;	// Increment index
 	}
 	
 	// Calculate the middle
 	midNum = i/2;
 	
 	// Prints out middle
 	printf("Middle item: %d\n", list[midNum]);
 	
 	// Success
 	return 0;
 
 }
