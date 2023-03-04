/**
 * Description: Coding practice
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 2, 2023
 * Creation date: March 3, 2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * This function prints out a statement four times using fork()
 * Assumption: none
 * Input parameters: none
 * Returns: none
 **/
int main(void) {

    fork();
    fork();
    printf("hello world from PID %ld!\n", (long)getpid());
    exit(0);
}




