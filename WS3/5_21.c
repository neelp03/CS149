/**
 * Description: Exercise 5.21 - "Contains the character" from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 14, 2023
 * Creation date: February 17, 2023
 **/
 
#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 10

/**
 * This function takes in a list of words and prints out
 * the words that contain a certain character
 * Assumption: At least one word in the list will contain 
 * the given character, the list will always contain less 
 * than 20 words, each word will contain less than 10 characters
 * and no spaces
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 
 	int totalWords;
 	int count;
 	char searchChar;
 	char list[19][10];
 	char temp[MAX_WORD_LENGTH];
 	
 	scanf("%d", &totalWords);
 	
 	for(int i = 0; i <= totalWords; i++){
 		
 		if(i == totalWords){
 			scanf(" %c", &searchChar);
 			break;
 		}
 		
 		scanf("%s", temp);
 		strcpy(list[i], temp);	
 	}
 	

 	
 	for(int x = 0; x < totalWords; x++){
 		if(strchr(list[x], searchChar) != NULL){
 			printf("%s,", list[x]);
 		}
 	}
 	
 	printf("\n");
 	return 0;
 
 }
