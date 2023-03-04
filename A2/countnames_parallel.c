/**
 * Description: This utility counts the number of times a name appears in a set of files
 * Author name: Neel Patel; Megan Ju
 * Author email: neel.patel01@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: March 2, 2023
 * Creation date: March 2, 2023
 **/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAME_LENGTH 30
#define MAX_NAMES 100

/**
 * count_name function for counting name occurrences in a file
 * Assumption: none
 * Input parameters: file name, write end of pipe
 * Returns: 0 if successful, 1 if there was an error
**/
int count_name(char *filename, int write_end) {
  // Declare two arrays to store names and their counts
  char names[MAX_NAMES][MAX_NAME_LENGTH];
  int counts[MAX_NAMES];

  // A variable to keep track of the number of names read from the file
  int name_count = 0;

  // Open the input file
  FILE *file = fopen(filename, "r");

  // If the file could not be opened, print an error message and return 1
  if (!file) {
    fprintf(stderr, "Error: Unable to open file %s.\n", filename);
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
      fprintf(stderr, "Warning - Line %d in %s is empty.\n", line_number, filename);
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

  // Write the count for each name to the parent process through the pipe
  for (int i = 0; i < name_count; i++) {
    char output[MAX_NAME_LENGTH + 10];  // Allocate space for the output string
    sprintf(output, "%s: %d\n", names[i], counts[i]);  // Format the output string
    write(write_end, output, strlen(output));  // Write the output string to the pipe
  }

  return 0;
}

/**
 * Main function for reading multiple files and counting name occurrences in parallel
 * Assumption: none
 * Input parameters: argc, argv
 * Returns: 0 if successful, 1 if there was an error
**/
int main(int argc, char *argv[]) {

}