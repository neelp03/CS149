/**
 * Description: Exercise 6.24 Swapping variables from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 15, 2023
 * Creation date: March 15, 2023
 **/
 
 #include <stdio.h>
 
  /**
 * This function swaps the first with the second and the third with the fourth values
 * Assumption: None
 * Input parameters: '*userVal1' - first value
 		     '*userVal2' - second value
 		     '*userVal3' - third value
  		     '*userVal4' - fourth value
 * Returns: none
 **/
 void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4){
 
 	int temp;
 	temp = *userVal2;
 	*userVal2 = *userVal1;
 	*userVal1 = temp;
 	temp = *userVal4;
 	*userVal4 = *userVal3;
 	*userVal3 = temp;
 }
 
 
 /**
 * This function calls the SwapValues function
 * Assumption: Reads only four integers
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	int first, second, third, fourth;
 	
 	scanf("%d %d %d %d", &first, &second, &third, &fourth);
 	
 	SwapValues(&first, &second, &third, &fourth);
 	
 	printf("%d %d %d %d\n", first, second, third, fourth);
 	
 	return 0;
 }
