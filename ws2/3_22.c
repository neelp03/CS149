/**
 * Description: Exercise 3.22 from Zybooks
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: February 8, 2023
 * Creation date: February 8, 2023
 **/

#include <stdio.h>
#include <math.h>

/**
 * This function finds the minimum of three values
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	int num1, num2, num3;
 	
 	//stores user input
 	scanf("%d", &num1);
 	scanf("%d", &num2);
 	scanf("%d", &num3);
 	
 	//creates an array with the numbers
 	int nums[3] = {num1, num2, num3};
 	
 	//storing the first value as the minimum
 	int min = num1;
 	
 	//for loop that checks the minimum
 	for(int i=1; i<3; i++){
 		if(nums[i] < min){
 			min = nums[i];
 		}
 	}
 	
 	//printing the smallest number
 	printf("%d\n", min);
 	
 	return 0;
 
 }
