#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_FILES 100
#define MAX_NAMES 100
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

int main(int argc, char *argv[])
{
    // Check that at least one file is provided
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        exit(1);
    }

    // Initialize variables
    int num_files = argc - 1;
    int pid[MAX_FILES];
    int pipes[MAX_FILES][2];
    char *names[MAX_NAMES];
    int name_count[MAX_NAMES] = {0};
    int num_names = 0;
    int name_idx[MAX_FILES];

    // Loop through all files and create child processes to count names
    for (int i = 0; i < num_files; i++)
    {
        // Create pipe for communication between parent and child processes
        if (pipe(pipes[i]) == -1)
        {
            perror("Error creating pipe");
            exit(1);
        }

        // Fork child process
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("Error forking process");
            exit(1);
        }
        else if (pid[i] == 0)
        {
            // Child process
            close(pipes[i][0]); // Close read end of pipe

            // Open file for reading
            FILE *fp = fopen(argv[i + 1], "r");
            if (fp == NULL)
            {
                perror("Error opening file");
                exit(1);
            }

            // Read file line by line and count names
            char line[MAX_LINE_LEN];
            while (fgets(line, MAX_LINE_LEN, fp) != NULL)
            {
                // Tokenize line by spaces
                char *token = strtok(line, " \t\n\r");
                while (token != NULL)
                {
                    // Check if token is a new name
                    int name_found = 0;
                    for (int j = 0; j < num_names; j++)
                    {
                        if (strcmp(token, names[j]) == 0)
                        {
                            name_found = 1;
                            name_idx[i] = j;
                            break;
                        }
                    }
                    if (!name_found)
                    {
                        // Add new name to list
                        names[num_names] = strdup(token);
                        name_count[num_names] = 0;
                        name_idx[i] = num_names;
                        num_names++;
                    }

                    // Increment count for name
                    name_count[name_idx[i]]++;

                    // Get next token
                    token = strtok(NULL, " \t\n\r");
                }
            }

            // Write count to pipe and exit
            if (write(pipes[i][1], &name_count[name_idx[i]], sizeof(int)) == -1)
            {
                perror("Error writing count to pipe");
                exit(1);
            }
            close(pipes[i][1]); // Close write end of pipe
            exit(0);
        }
        else
        {
            // Parent process
            close(pipes[i][1]); // Close write end of pipe
        }
    }

    // Loop through child processes and read counts from pipes
    for (int i = 0; i < num_files; i++)
    {
        // Wait for child process to finish
        int status;
        waitpid(pid[i], &status, 0);

        // Read count from pipe
        int count;
        if (read(pipes[i][0], &count, sizeof(int)) == -1)
        {
            perror("Error reading count from pipe");
            exit(1);
        }
        close(pipes[i][0]); // Close read end of pipe

        // Print count for name
        printf("%s: %d\n", names[name_idx[i]], count);
    }
    return 0;
}