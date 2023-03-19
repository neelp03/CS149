/**
 * Description: This utility executes multiple commands
 * Author name: Neel Patel; Megan Ju
 * Author email: neel.patel01@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: March 18, 2023
 * Creation date: March 7, 2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MAX_ARGS 100
#define MAX_LINE 1000

/**
 * log_message - appends a message to a file
 * Assumption: The file specified by the filename parameter must exist
 * or be created with read and write permissions for the user and read
 * permissions for the group and others.
 * @param filename: The name of the file to which the message will be appended
 * @param message: The message to be appended to the file
 * Returns: void
**/
void log_message(char * filename, char * message) {
  int fd;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  
  // Open file for reading and writing, creating it if necessary. Append to end of file.
  if ((fd = open(filename, O_RDWR | O_CREAT | O_APPEND, mode)) < 0) {
    perror("open");
    exit(1);
  }
  
  // write message to file
  if (write(fd, message, strlen(message)) < 0) {
    perror("write");
    exit(1);
  }
  
  // close file descriptor
  close(fd);
}

/**
 * main - runs a loop that reads a command from standard input,
 * executes the command as a child process and logs its output and errors
 * Assumption: None
 * @param argc: The number of command-line arguments
 * @param argv: An array of command-line argument strings
 * Returns: 0 on successful execution, or 1 on error
**/	
int main(int argc, char ** argv) {
  char * line = NULL;
  size_t line_buf_size = 0;
  ssize_t line_size;
  int index = 1; // counter for command index
  int pid, status;
  char out_file[256], err_file[256], message[256], * arg[MAX_ARGS];

  while ((line_size = getline( & line, & line_buf_size, stdin)) != -1) {
    if (line_size > MAX_LINE) {
      fprintf(stderr, "Line too long: %s", line);
      exit(1);
    }
    int arg_count = 0;
    arg[arg_count++] = strtok(line, " \n\t"); // tokenized input line
    while ((arg[arg_count++] = strtok(NULL, " \n\t")) != NULL) { // check if there are too many arguments
      if (arg_count == MAX_ARGS) {
        fprintf(stderr, "Too many arguments: %s", line);
        exit(1);
      }
    }
    arg[arg_count] = NULL;

    pid = fork(); // fork a child process
    if (pid < 0) {
      perror("fork");
      exit(1);
    } else if (pid == 0) { // child process
      // creates output and error files
      snprintf(out_file, sizeof(out_file), "%d.out", getpid());
      snprintf(err_file, sizeof(err_file), "%d.err", getpid());
      int out_fd, err_fd;
      mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      if ((out_fd = open(out_file, O_RDWR | O_CREAT | O_APPEND, mode)) < 0) {
        perror("open");
        exit(1);
      }
      if ((err_fd = open(err_file, O_RDWR | O_CREAT | O_APPEND, mode)) < 0) {
        perror("open");
        exit(1);
      }
      if (dup2(out_fd, 1) < 0) { // stdout to output file
        perror("dup2");
        exit(1);
      }
      if (dup2(err_fd, 2) < 0) { // stderr to error file
        perror("dup2");
        exit(1);
      }
      close(out_fd);
      close(err_fd);
      snprintf(message, sizeof(message), "Starting command %d: child %d pid of parent %d\n", index, getpid(), getppid());
      log_message(out_file, message);
      if (execvp(arg[0], arg) < 0) { // execute command
        snprintf(message, sizeof(message), "%s: %s\n", arg[0], strerror(errno));
        log_message(err_file, message); // log error
        exit(2);
      }
    } else { // parent
      wait( & status); // wait for child to finish
      if (WIFEXITED(status)) { // check if child exited normally
        snprintf(out_file, sizeof(out_file), "%d.out", pid); // get output file name
        snprintf(err_file, sizeof(err_file), "%d.err", pid); // get error file name
	// log exit message
        snprintf(message, sizeof(message), "Finished child %d pid of parent %d\n", pid, getpid());
        log_message(out_file, message);
        log_message(err_file, "Exited with exitcode = 0\n");
      } else if (WIFSIGNALED(status)) {
        snprintf(err_file, sizeof(err_file), "%d.err", pid);
        log_message(err_file, "Killed with signal 15\n");
      }
    }
    index++; // increment the command index
  }

  free(line); // deallocates memory to prevent leaks and overflow
  return 0;
}
