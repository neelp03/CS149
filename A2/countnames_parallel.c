/**
 * Description: This utility counts the number of times a name appears in a set of files
 * Author name: Neel Patel; Megan Ju
 * Author email: neel.patel01@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: March 2, 2023
 * Creation date: March 2, 2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NAME_LENGTH 30
#define MAX_NAMES 100

/**
 * Main function for counting name occurrences in files
 * Assumption: none
 * Input parameters: argc, argv
 * Returns: 0 if successful, 1 if there was an error
**/
int main(int argc, char *argv[]) {
  int i, j, status;
  char name[MAX_NAME_LENGTH];
  int counts[MAX_NAMES];
  pid_t pid;
  int name_count = 0;
  int pipefd[2];

  // Create the pipe for communication between parent and child processes
  if (pipe(pipefd) == -1) {
    perror("pipe");
    return 1;
  }

  // Fork child processes to count names in each file
  for (i = 1; i < argc; i++) {
    pid = fork();

    if (pid == -1) {
      perror("fork");
      return 1;
    }

    if (pid == 0) {
      // Child process
      FILE *file = fopen(argv[i], "r");

      if (!file) {
        fprintf(stderr, "Error: Unable to open file %s.\n", argv[i]);
        exit(1);
      }

      while (fgets(name, MAX_NAME_LENGTH, file)) {
        int length = strlen(name);
        if (length > 1) {
          if (name[length - 1] == '\n') {
            name[length - 1] = '\0';
          }

          int existing_name = 0;
          for (j = 0; j < name_count; j++) {
            if (strcmp(name, argv[j+1]) == 0) {
              counts[j]++;
              existing_name = 1;
              break;
            }
          }

          if (!existing_name) {
            strcpy(argv[name_count+1], name);
            counts[name_count] = 1;
            name_count++;
          }
        }
      }

      fclose(file);

      // Write the count for each name to the parent process through the pipe
      write(pipefd[1], counts, MAX_NAMES * sizeof(int));
      exit(0);
    }
  }

  // Parent process
  for (i = 1; i < argc; i++) {
    wait(&status);
    if (WIFEXITED(status)) {
      read(pipefd[0], counts, MAX_NAMES * sizeof(int));
      for (j = 0; j < name_count; j++) {
        printf("%s: %d\n", argv[j+1], counts[j]);
      }
    }
  }

  return 0;
}
