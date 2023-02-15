/**
 * Description: Exercise 5.21 - "Contains the character" from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 14, 2023
 * Creation date: February 14, 2023
 **/
 
#include <stdio.h>

/**
 * This function takes in a list of words and prints out
 * the words that contain a certain character
 * Assumption: At least one word in the list will contain 
 * the given character, the list will always contain less 
 * than 20 words
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 
 	int totalWords;
 	char searchChar;
 	char list[19][10];
 	char temp;
 	
 	scanf("%d", totalWords);
 	
 	for(int i = 0; i <= totalWords; i++){
 		if(i == totalWords){
 			scanf("%c", searchChar);
 			break;
 		}
 		
 		
 		
 		list[i] = 
 	}
 	
 	
 
 
 }
