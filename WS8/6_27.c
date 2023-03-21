/**
 * Description: Exercise 6.27 Count characters - functions from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 21, 2023
 * Creation date: March 21, 2023
 **/
 
#include <stdio.h>
#include <string.h>

/**
 * This function counts the number of times a character
 * appears in a string
 * Assumption: Input does not contain spaces
 * and will contain less than 50 characters
 * Input parameters: none
 * Returns: an integer
 **/
 
 int main(){
 
 	char ch;
 	char x;
 	int count = 0;
 	
 	ch=getchar();
 	
 	while((x=getchar()) != '\n'){
		if(x == ch){
			count++;
		}
	}
 	
 	if(count==1){
 		printf("%d %c\n", count, ch);	
 	}
 	else {
 		printf("%d %c's\n", count, ch);
 	}
 	
 	return 0;
 }
