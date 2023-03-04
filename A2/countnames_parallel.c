/**
 * Description: This utility counts the number of times a name appears in a set of files
 * Author name: Neel Patel; Megan Ju
 * Author email: neel.patel01@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: March 2, 2023
 * Creation date: March 2, 2023
 **/#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_NAME_LENGTH 30
#define MAX_NAMES 100

/**
 * Count the number of occurrences of each name in a file
 * Assumption: none
 * Input parameters: file name, name_counts array
 * Returns: 0 if successful, 1 if there was an error
**/
int count_names_in_file(char *file_name, int *name_counts) {
  // Declare an array to store names and their counts
  char names[MAX_NAMES][MAX_NAME_LENGTH];
  int counts[MAX_NAMES];

  // A variable to keep track of the number of names read from the file
  int name_count = 0;

  // Open the input file
  FILE *file = fopen(file_name, "r");

  // If the file could not be opened, print an error message and return 1
  if (!file) {
    fprintf(stderr, "Error: Unable to open file %s.\n", file_name);
    return 1;
  }

  // Read the names from the input file
  char name[MAX_NAME_LENGTH];
  int line_number = 0;
  while (fgets(name, MAX_NAME_LENGTH, file)) {
    // Increment the line number
    line_number++;

    // Get the length of the name read
    int length = strlen(name);

    // If the length of the name is 1 or less, it is considered an empty line
    // Print a warning message and continue with the next iteration of the loop
    if (length <= 1) {
      fprintf(stderr, "Warning - Line %d in file %s is empty.\n", line_number, file_name);
      continue;
    }

    // Remove the newline character from the name
    if (name[strlen(name) - 1] == '\n') {
      name[strlen(name) - 1] = '\0';
    }

    // Check if the name already exists in the names array
    int existing_name = 0;
    for (int i = 0; i < name_count; i++) {
      if (strcmp(names[i], name) == 0) {
        // If the name exists, increment its count
        counts[i]++;
        existing_name = 1;
        break;
      }
    }

    // If the name does not exist in the names array, add it to the array
    if (!existing_name) {
      strcpy(names[name_count], name);
      counts[name_count] = 1;
      name_count++;
    }
  }

  // Close the input file
  fclose(file);

  // Write the count for each name to the name_counts array
  for (int i = 0; i < name_count; i++) {
    name_counts[i * 2] = counts[i];
    strcpy((char *)&name_counts[i * 2 + 1], names[i]);
  }

  // Return 0 to indicate that the program has executed successfully
  return 0;
}

/**
 * Main function for counting name occurrences in multiple files
 * Assumption: none
 * Input parameters: argc, argv
 * Returns: 0 if successful, 1 if there was an error
**/
int main(int argc, char *argv[]) {
    int i, j, pid, status;
    int name_counts[MAX_NAMES * 2] = {0}; // initialize to 0
    int total_counts[MAX_NAMES] = {0}; // initialize to 0
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            close(pipefd[0]); // Close the read end of the pipe
            int ret = count_names_in_file(argv[i], name_counts);
            if (ret != 0) {
                fprintf(stderr, "Error counting names in file %s\n", argv[i]);
                exit(EXIT_FAILURE);
            }
            // Write the name counts to the parent process through the pipe
            write(pipefd[1], name_counts, MAX_NAMES * 2 * sizeof(int));
            close(pipefd[1]); // Close the write end of the pipe
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(pipefd[1]); // Close the write end of the pipe
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
                // Read the name counts from the child process
                read(pipefd[0], name_counts, MAX_NAMES * 2 * sizeof(int));
                // Accumulate the name counts across all files
                for (j = 0; j < MAX_NAMES; j++) {
                    total_counts[j] += name_counts[j * 2 + 1];
                }
            }
            close(pipefd[0]); // Close the read end of the pipe
        }
    }

    // Print the collective number of names across all files
    for (j = 0; j < MAX_NAMES; j++) {
        if (total_counts[j] > 0) {
            printf("%s: %d\n", (char*)name_counts[j*2], total_counts[j]);
        }
    }

    return 0;
}
