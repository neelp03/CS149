/**
  * Description: This utility counts the number of times a name appears in a set of files
  * Author name: Neel Patel; Megan Ju
  * Author email: neel.patel01@sjsu.edu; megan.ju@sjsu.edu
  * Last modified date: April 25, 2023
  * Creation date: April 25, 2023
  **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAME_LEN 100
#define MAX_FILES 10

// created structure to contain
// names and counts
struct name_count {
    char name[MAX_NAME_LEN];
    int count;
};

/**
 * Main function for counting name occurrences in multiple files
 * Assumption: none
 * Input parameters: argc, argv
 * Returns: 0 if successful, 1 if there was an error
**/
int main(int argc, char *argv[]) {

    // number of files passed into the method
    int num_files = argc - 1;
    
    /* if number of files is bigger than accepted,
      print out an error and exit the method */
    if (num_files > MAX_FILES) {
        fprintf(stderr, "Error: too many files (maximum %d)\n", MAX_FILES);
        exit(1);
    }

    // creating pipes for each file that was passed in
    int pipes[num_files][2];
    // keeping track of process ids for each file
    pid_t pids[num_files];

/*---  ITERATING THROUGH EACH FILE TO COUNT NAMES  ---*/

    for (int i = 0; i < num_files; i++) {
    	// if there's an error with the pipe, 
    	// exit the method
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
        // creating a new process
        pid_t pid = fork();
        
        // if fork() failed
        if (pid == -1) {
            perror("fork");
            exit(1);
        } 
        
        /*****  CHILD PROCESS  *****/
        else if (pid == 0) {
            // closing read end of the pipe
            close(pipes[i][0]);
            // open input file
            FILE *file = fopen(argv[i+1], "r");
            // If file is null, exit the method
            if (file == NULL) {
                perror("fopen");
                exit(1);
            }
            
            // keep track of name counts for each name read
            struct name_count counts[MAX_NAME_LEN] = {0};
            // temp var for name read
            char line[MAX_NAME_LEN];
            
            // read until eof
            while (fgets(line, sizeof(line), file) != NULL) {
    		// store the first name found by breaking up the read line
    		// by tab, carriage returns, and newlines        
    		char *first_name = strtok(line, " \t\r\n");
    		// store the last name found by breaking up the prior read in
    		// line by tab, carriage returns, and newlines  
    		char *last_name = strtok(NULL, " \t\r\n");
    		
    		// as long as there is a first name and last name
    		while (first_name != NULL && last_name != NULL) {
    			// temp var for name
        		char name[MAX_NAME_LEN];
        		// stores name found in the temp variable
        		snprintf(name, MAX_NAME_LEN, "%s %s", first_name, last_name);
        		
        		// keeping track if the name was found or not
        		int found = 0;
        		// for each name
        		for (int j = 0; j < MAX_NAME_LEN; j++) {
        			
        			// if the count for the name is 0
            			if (counts[j].count == 0) {
            			// copy the name into the counts structure
                		strcpy(counts[j].name, name);
                		// update count
                		counts[j].count = 1;
                		// found is updated
                		found = 1;
                		break;
            			} 
            			
            			// if the name is already in counts
            			else if (strcmp(counts[j].name, name) == 0) {
            				// update count
                			counts[j].count++;
                			// found is updated
                			found = 1;
                			break;
            			}
        		}
        		// if the name isn't found, error printed
        		if (!found) {
            			fprintf(stderr, "Error: too many names\n");
            			exit(1);
        		}
        		
        		// read in new first and last names in same way as before
        		first_name = strtok(NULL, " \t\r\n");
        		last_name = strtok(NULL, " \t\r\n");
    		}	
	}

            // closing the file
            fclose(file);
            // for each name
            for (int j = 0; j < MAX_NAME_LEN; j++) {
            	// if the name exists
                if (counts[j].count > 0) {
                    char buf[MAX_NAME_LEN+10];
                    // store name in counts
                    sprintf(buf, "%s: %d", counts[j].name, counts[j].count);
                    // if writing through the write end of the pipe occurs an error
                    // print out error
                    if (write(pipes[i][1], buf, sizeof(buf)) == -1) {
                        perror("write");
                        exit(1);
                    }
                }
            }
            // closing write end of pipe
            close(pipes[i][1]);
            // successful process
            exit(0);
        } 
        
        /*****  PARENT PROCESS  *****/
        else {
            // save pid in array
            pids[i] = pid;
            // close write end of the pipe
            close(pipes[i][1]);
        }
    }


    /*---  READING NAMES COUNTS FROM CHILD PROCESSES AND COMBINE  ---*/
    // combined_counts keeps track of the name and the total count found
    struct name_count combined_counts[MAX_NAME_LEN] = {0};
    
    // for each file
    for (int i = 0; i < num_files; i++) {
    	
    	// buffer
        char buf[MAX_NAME_LEN+10];
        
        // get value read from pipes for certain file,
        // read until eof
        while (read(pipes[i][0], buf, sizeof(buf)) > 0) {
            
            // breaks the read string up by :
            // saves name to the name in combined_counts
            char *name = strtok(buf, ":");
         
            // breaks the previous string up by :
            // saves count to the count_str in combined_counts
            char *count_str = strtok(NULL, ":");
            
            // convert string argument to integer
            int count = atoi(count_str);
        
            // new names found	
            int found = 0;
        
        // for each name in the file
        for (int j = 0; j < MAX_NAME_LEN; j++) {
            // if the name hasn't been found before, copy the name into
            // combine_counts and set the count to updated count value.
            // Update found value and go to next iteration
            if (combined_counts[j].count == 0) {
                strcpy(combined_counts[j].name, name);
                combined_counts[j].count = count;
                found = 1;
                break;
            } 
            
            // if the name already matches another name already found
            // update count for that name. Update found value and go to 
            // next iteration
            else if (strcmp(combined_counts[j].name, name) == 0) {
                combined_counts[j].count += count;
                found = 1;
                break;
            }
        }
        
        // If found is 0, there are too many names. Printing error
        if (!found) {
            fprintf(stderr, "Error: too many names\n");
            exit(1);
        }
    }
    // close read end of each pipe
    close(pipes[i][0]);
}

/*---  WAIT FOR CHILD TO FINISH  ---*/
// for each file
for (int i = 0; i < num_files; i++) {
    int status;
    // waiting for child processes, print error to
    // stderr
    if (waitpid(pids[i], &status, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    // If there was a problem with child termination, print status 
    // error to stderr
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "Error: child process exited with status %d\n", status);
        exit(1);
    }
}

/*---  PRINTING OUT THE COMBINED NAME COUNTS  ---*/
for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (combined_counts[i].count > 0) {
        printf("%s: %d\n", combined_counts[i].name, combined_counts[i].count);
    }
}

// successful program
return 0;
}

