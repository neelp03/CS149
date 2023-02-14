/**
 * Description: Exercise 5.16 - Output numbers in reverse from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 13, 2023
 * Creation date: February 13, 2023
 **/
 
#include <stdio.h>

/**
 * This function prints out an integer in reverse binary form
 * Assumption: List will always contain less than 20 integers
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	
 	// Obtain the amount of numbers in the list
 	int length;
 	scanf("%d", &length);

 
 	// Create array for output
 	int list[length];
 	int temp;	// Temporary variable to hold values from list
 	int threshold; //Threshold value
 	
 	// Put the numbers in the list into an array
 	for(int i = 0; i < length+1; i++){
 		if(i == length){
 			scanf("%d", &threshold);
 			break;
 		} else {
 			scanf("%d", &temp);
 			list[i] = temp;
 		}
 	}
 	
 	// Print out the values that are less than or 
 	// meet the threshold
 	for(int j = 0; j < length; j++){ 		
 		if(list[j] <= threshold){
 			printf("%d,", list[j]);
 		}
 	}
 	
 	printf("\n");
 	return 0;	// Successful execution
 	
 }
