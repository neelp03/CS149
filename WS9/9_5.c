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

int main() {

	char input[MAX_LENGTH + 1];
	char ch[2] = ",";
	char *first, *second, *token;
	char second[MAX_LENGTH];
	int ret;
	char stop[2] = "q";
	bool done = false;

	
	while(!done){
	
		printf("Enter input string:\n");
		fgets(input, MAX_LENGTH+1, stdin);
	
		if(strcmp(input, stop) == 0){
			done = true;
			exit(0);
		}
	
		if(ret = strchr(input, ch) == NULL){
			fprintf(stderr, "Error: No comma in string.\n");
		}
		else{
			printf("Hello\n");
			token = strtok(input, ch);
			first = token;
			if(token!=NULL){
				token = strtok(NULL, ch);
				second = token;
			} 
		}
		
	}

	return 0;
}
