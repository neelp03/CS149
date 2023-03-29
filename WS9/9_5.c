#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Parses string to output the two strings separated by a comma
 * Assumption: If the input contains a comma, the input also contains two strings
 * Input parameters: none
 * Returns: None.
 */
int main() {

    char buffer[32];
    char *b = buffer;
    char *first, *second, *token;
    size_t bufsize = 32;
    char ch[2] = ",";
    char quit[3] = "q1";
    int loop = 1;

while(loop){

    printf("Enter input string:\n");
    getline(&b, &bufsize, stdin);

    if(strcmp(buffer, quit) == 0){
    	printf("not working\n");
    	loop = 0;
    }
    
    else {
    
    	if(strchr(buffer, (int)ch[0]) == NULL){
    		fprintf(stderr, "Error: No comma in string.\n");
    	}
	
    	else {
    		token = strtok(buffer, ch);
    		first = token;
    		if(token!=NULL){
    			token = strtok(NULL, ch);
    			second = token;
    		}
			
		printf("First word: %s\n", first);
		printf("Second word: %s\n", second); 
	}

    }
    
}

    return(0);
}
