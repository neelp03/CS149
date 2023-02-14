/**
 * Description: Exercise 4.14 - Convert to reverse binary from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 13, 2023
 * Creation date: February 13, 2023
 **/
 
#include <stdio.h>
#include <math.h>

#define MAX_BINARY_LENGTH 64

/**
 * This function prints out an integer in reverse binary form
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	int num = 0;
 	int binNumLength = 0;
 	char output[MAX_BINARY_LENGTH];
 	
 	scanf("%d", &num);
 	
 	// Function only takes in positive integers
 	if(num<0){
 		return 1;
 	}
 
 	while(num>0){
 		output[binNumLength] = num%2;
 		num = num / 2;
 		binNumLength++;
 	}
 	
 	for(int i = 0; i< binNumLength; i++){
 		printf("%d", output[i]);
 	}
 	
 	printf("\n");
 	return 0;
 
 
 }
