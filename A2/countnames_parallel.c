#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAMES 1000
#define MAX_NAME_LEN 50
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
    // Declare names and counts arrays to hold names and counts
    char names[MAX_NAMES][MAX_NAME_LEN];
    int counts[MAX_NAMES] = {0};
    int num_names = 0;

    // Iterate over files and count names
    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        if (!file)
        {
            perror(argv[i]);
            continue;
        }

        char buf[BUF_SIZE];
        while (fgets(buf, BUF_SIZE, file))
        {
            // Trim trailing newline
            buf[strcspn(buf, "\n")] = '\0';

            // Check if name already exists in names array
            int found = 0;
            for (int j = 0; j < num_names; j++)
            {
                if (strcmp(buf, names[j]) == 0)
                {
                    found = 1;
                    counts[j]++;
                    break;
                }
            }

            // If name doesn't exist in names array, add it
            if (!found)
            {
                strcpy(names[num_names], buf);
                counts[num_names]++;
                num_names++;
            }
        }

        fclose(file);
    }

    // Create pipes for communicating counts from child processes to parent process
    int fd[num_names][2];
    for (int i = 0; i < num_names; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
    }

    // Fork child processes to count names
    for (int i = 0; i < num_names; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        { // Child process
            // Close unused write end of pipe
            close(fd[i][1]);

            // Read from file and count name
            int count = 0;
            FILE *file = fopen(argv[1], "r");
            if (!file)
            {
                perror(argv[1]);
                exit(1);
            }

            char buf[BUF_SIZE];
            while (fgets(buf, BUF_SIZE, file))
            {
                // Trim trailing newline
                buf[strcspn(buf, "\n")] = '\0';

                if (strcmp(buf, names[i]) == 0)
                {
                    count++;
                }
            }

            fclose(file);

            // Write count to pipe
            write(fd[i][0], &count, sizeof(int));

            // Close read end of pipe
            close(fd[i][0]);

            // Exit child process
            exit(0);
        }
        else
        { // Parent process
            // Close unused read end of pipe
            close(fd[i][0]);
        }
    }

    // Wait for child processes to finish and read counts from pipes
    // Loop through child processes and read counts from pipes
    for (int i = 0; i < num_files; i++)
    {
        int status;
        if (waitpid(pid[i], &status, 0) == -1)
        {
            fprintf(stderr, "Error waiting for child process %d\n", pid[i]);
            continue;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            int count;
            if (read(pipes[i][0], &count, sizeof(count)) == -1)
            {
                fprintf(stderr, "Error reading count from pipe %d\n", i);
                continue;
            }
            name_count[name_idx[i]] += count;
        }
        else
        {
            fprintf(stderr, "Child process %d exited with error status %d\n", pid[i], WEXITSTATUS(status));
        }
        close(pipes[i][0]);
    }
    // Print final counts for each name
    for (int i = 0; i < num_names; i++)
    {
        printf("%s:%d\n", names[i], name_count[i]);
    }
    return 0;
}
