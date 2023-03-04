#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_NAMES 1000
#define MAX_NAME_LENGTH 50

int count_names(const char *filename, char names[][MAX_NAME_LENGTH], int counts[], int *name_count);
int find_name_index(char names[][MAX_NAME_LENGTH], int name_count, const char *name, int *index);

int main(int argc, char *argv[]) {
  // Check the number of arguments passed to the program
  // If the number of arguments is not 2 or more, then print an error message and return 1
  if (argc < 2) {
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    return 1;
  }

  // Declare two arrays to store names and their counts
  char names[MAX_NAMES][MAX_NAME_LENGTH];
  int counts[MAX_NAMES];
  int name_count = 0;

  // Create a pipe for each child process to write the result back to the parent process
  int num_pipes = argc - 1;
  int pipes[num_pipes][2];
  for (int i = 0; i < num_pipes; i++) {
    if (pipe(pipes[i]) == -1) {
      perror("Error creating pipe");
      exit(1);
    }
  }

  // Fork a child process for each file
  for (int i = 1; i < argc; i++) {
    pid_t pid = fork();
    if (pid == -1) {
      perror("Error forking process");
      exit(1);
    } else if (pid == 0) {
      // Child process

      // Close the read end of the pipe
      close(pipes[i - 1][0]);

      // Count the names in the file
      int name_count_child = 0;
      int counts_child[MAX_NAMES];
      char names_child[MAX_NAMES][MAX_NAME_LENGTH];
      int ret = count_names(argv[i], names_child, counts_child, &name_count_child);

      // If there was an error, write 1 to the pipe and exit
      if (ret != 0) {
        write(pipes[i - 1][1], "1", 1);
        exit(1);
      }

      // Write the count data to the pipe
      char buffer[MAX_NAMES * (MAX_NAME_LENGTH + 4)]; // Allocate enough memory to hold all the data
      int bytes_written = 0;
      for (int j = 0; j < name_count_child; j++) {
        bytes_written += sprintf(buffer + bytes_written, "%s: %d\n", names_child[j], counts_child[j]);
      }
      write(pipes[i - 1][1], buffer, bytes_written);

      // Close the write end of the pipe and exit
      close(pipes[i - 1][1]);
      exit(0);
    } else {
      // Parent process

      // Close the write end of the pipe
      close(pipes[i - 1][1]);

      // Wait for the child process to exit and read the count data from the pipe
      wait(NULL);
      char buffer[MAX_NAMES * (MAX_NAME_LENGTH + 4)]; // Allocate enough memory to hold all the data
      int bytes_read = read(pipes[i - 1][0], buffer, sizeof(buffer));
      buffer[bytes_read] = '\0';

      // Check if the child process returned an error
int status;
if (waitpid(pid, &status, 0) == -1) {
perror("Error waiting for child process");
exit(1);
}
if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
fprintf(stderr, "Error: Unable to count names in file %s\n", argv[i]);
continue;
}
  // Parse the count data from the buffer and update the global counts
  char *line = strtok(buffer, "\n");
  while (line != NULL) {
    char *name = strtok(line, ": ");
    int count = atoi(strtok(NULL, ": "));
    int index;
    if (find_name_index(names, name_count, name, &index) == 0) {
      counts[index] += count;
    } else {
      strncpy(names[name_count], name, MAX_NAME_LENGTH);
      counts[name_count] = count;
      name_count++;
    }
    line = strtok(NULL, "\n");
  }

  // Close the read end of the pipe
  close(pipes[i - 1][0]);
}
}

// Print the global counts
for (int i = 0; i < name_count; i++) {
printf("%s: %d\n", names[i], counts[i]);
}

return 0;
}

int count_names(const char *filename, char names[][MAX_NAME_LENGTH], int counts[], int *name_count) {
FILE *file = fopen(filename, "r");
if (file == NULL) {
perror("Error opening file");
return 1;
}

char name[MAX_NAME_LENGTH];
while (fscanf(file, "%s", name) == 1) {
int index;
if (find_name_index(names, *name_count, name, &index) == 0) {
counts[index]++;
} else {
strncpy(names[*name_count], name, MAX_NAME_LENGTH);
counts[*name_count] = 1;
(*name_count)++;
}
}

if (fclose(file) == EOF) {
perror("Error closing file");
return 1;
}

return 0;
}

int find_name_index(char names[][MAX_NAME_LENGTH], int name_count, const char *name, int *index) {
for (int i = 0; i < name_count; i++) {
if (strcmp(names[i], name) == 0) {
*index = i;
return 0;
}
}
return 1;
}

