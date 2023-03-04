/**
 * Description: Exercise 6.29 Convert to binary from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 3, 2023
 * Creation date: March 3, 2023
 **/
 
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 64

int numLength = 0;

/**
 * This function calculates backwards
 * binary form
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
int IntToReverseBinary(int integerValue, char binaryValue[MAX_LENGTH]){

	while(integerValue>0){
		binaryValue[numLength] = integerValue%2;
		integerValue = integerValue/2;
		numLength++;
	}
	
	return 0;
}

/**
 * This function reverses backwards binary
 * string to correct form
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
int StringReverse(char inputString[MAX_LENGTH], char reverseString[MAX_LENGTH]){
	
	int index = 0;
	for(int i = numLength-1; i >=0; i--){
		inputString[index] = reverseString[i];
		index++;
	}
	
	return 0;
}

/**
 * This function converts a base 10 number
 * to it's correct binary form
 * Assumption: Input will not exceed 50 chars
 * Input parameters: none
 * Returns: an integer
 **/
int main(void) {

	char backwards[MAX_LENGTH];
	char correct[MAX_LENGTH];
	int num = 0;
	
	scanf("%d", &num);
	IntToReverseBinary(num, backwards);
	StringReverse(correct, backwards);
	
	for(int i = 0; i<numLength; i++){
		printf("%d", correct[i]);
	}
	
	printf("\n");
	
	return 0;
	
}




