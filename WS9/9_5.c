/**
 * Description: Exercise 9.5 - Parsing strings from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 28, 2023
 * Creation date: March 27, 2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 50

/**
 * Parses string to output the two strings separated by a comma
 * Assumption: If the input contains a comma, the input also contains two strings
 * Input parameters: none
 * Returns: None.
 */
int main() {

	char input[MAX_LENGTH + 1];
	char ch[2] = ",";
	char *first, *second, *token;
	char stop[2] = "q";
	bool done = false;

	
	while(!done){
	
		printf("Enter input string:\n");
		fgets(input, MAX_LENGTH+1, stdin);
	
		if(strcmp(input, stop) == 0){
			done = true;
			break;
		}
	
		if(strchr(input, (int)ch[0]) == NULL){
			fprintf(stderr, "Error: No comma in string.\n");
		}
		else{
			token = strtok(input, ch);
			first = token;
			if(token!=NULL){
				token = strtok(NULL, ch);
				second = token;
			}
			
			printf("First word: %s\n", first);
			printf("Second word: %s\n", second); 
		}
		
	}

	return 0;
}
