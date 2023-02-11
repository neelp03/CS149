/**
 * Description: This utility counts the number of times a name appears in a given file
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel01@sjsu.edu; megan.ju@gmail.com
 * Last modified date: February 10, 2023
 * Creation date: February 10, 2023
 **/

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 30
#define MAX_NAMES 100

/**
 * Main function for counting name occurrences in a file
 * Assumption: none
 * Input parameters: argc, argv
 * Returns: 0 if successful, 1 if there was an error
**/
int main(int argc, char *argv[]) {
  // Check the number of arguments passed to the program
  // If the number of arguments is not 2, then print an error message and return 1
  if (argc != 2) {
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
    return 1;
  }

  // Declare two arrays to store names and their counts
  char names[MAX_NAMES][MAX_NAME_LENGTH];
  int counts[MAX_NAMES];

  // A variable to keep track of the number of names read from the file
  int name_count = 0;

  // Open the input file
  FILE *file = fopen(argv[1], "r");

  // If the file could not be opened, print an error message and return 1
  if (!file) {
    fprintf(stderr, "Error: Unable to open file %s.\n", argv[1]);
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
      fprintf(stderr, "Warning - Line %d is empty.\n", line_number);
      continue;
    }

    // Remove the newline character from the name
    name[length - 1] = '\0';

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

  // Print the count for each name
  for (int i = 0; i < name_count; i++) {
    printf("%s: %d\n", names[i], counts[i]);
  }

  // Return 0 to indicate that the program has executed successfully
  return 0;
}

