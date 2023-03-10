/**
 * Description: Exercise 5.22 - "Element in Range" from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 17, 2023
 * Creation date: February 15, 2023
 **/
 
#include <stdio.h>

/**
 * Description: This function prints out all numbers
 * that are in range of two given numbers (inclusive)
 * Input parameters: none
 * Returns: an integer
 **/
int main(void){ 
	 int n;
	 scanf("%d", &n);
	 int input[n];
	 int lo, hi;
	 for(int i = 0; i < n; i++) {
	 	scanf("%d", &input[i]);
	 }
	scanf("%d", &lo);
	scanf("%d", &hi);
	
	for(int i = 0; i < n; i++) {
		if(input[i] >= lo && input[i] <= hi) {
			printf("%d,", input[i]);
		}
	}
	
	printf("\n");
	return 0;
}

