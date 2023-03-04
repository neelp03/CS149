#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAME_LEN 30
#define MAX_FILES 10

struct NameCount
{
    char name[MAX_NAME_LEN];
    int count;
};

int compare_names(const void *a, const void *b)
{
    const struct NameCount *na = (const struct NameCount *)a;
    const struct NameCount *nb = (const struct NameCount *)b;
    return strcmp(na->name, nb->name);
}

int read_names(const char *filename, struct NameCount *names, int *num_names)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return -1;
    }

    char line[MAX_NAME_LEN];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Remove leading and trailing whitespace
        char *start = line;
        char *end = start + strlen(line) - 1;
        while (isspace(*start))
            start++;
        while (isspace(*end))
            end--;
        *(end + 1) = '\0';

        // Search for name in list
        int found = 0;
        for (int i = 0; i < *num_names; i++)
        {
            if (strcmp(names[i].name, start) == 0)
            {
                names[i].count++;
                found = 1;
                break;
            }
        }

        // Add name to list if not found
        if (!found)
        {
            strncpy(names[*num_names].name, start, MAX_NAME_LEN);
            names[*num_names].count = 1;
            (*num_names)++;
        }
    }

    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    int num_files = argc - 1;
    if (num_files > MAX_FILES)
    {
        fprintf(stderr, "Too many input files\n");
        exit(EXIT_FAILURE);
    }

    int num_names = 0;
    struct NameCount names[MAX_NAME_LEN];

    int fd[MAX_FILES][2];
    pid_t pid[MAX_FILES];

    // Create pipes and fork child processes
    for (int i = 0; i < num_files; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0)
        {
            // Child process
            close(fd[i][0]); // Close read end of pipe

            // Read names from file and write to pipe
            if (read_names(argv[i + 1], names, &num_names) == -1)
            {
                fprintf(stderr, "Error reading file: %s\n", argv[i + 1]);
                exit(EXIT_FAILURE);
            }
            if (write(fd[i][1], &num_names, sizeof(num_names)) == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
            if (write(fd[i][1], names, num_names * sizeof(struct NameCount)) == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }

            close(fd[i][1]); // Close write end of pipe
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(fd[i][1]); // Close write end of pipe for this child process

            int status;
            waitpid(pid[i], &status, 0); // Wait for child process to finish

            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            {
                // Read data from pipe
                int num_names_child;
                if (read(fd[i][0], &num_names_child, sizeof(num_names_child)) == -1)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                if (read(fd[i][0], names, num_names_child * sizeof(struct NameCount)) == -1)
                {
                    perror("read");
                    exit(EXIT_FAILURE);
                }

                // Merge child process data with parent process data
                for (int j = 0; j < num_names_child; j++)
                {
                    int found = 0;
                    for (int k = 0; k < num_names; k++)
                    {
                        if (strcmp(names[j].name, names[k].name) == 0)
                        {
                            names[k].count += names[j].count;
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                    {
                        strncpy(names[num_names].name, names[j].name, MAX_NAME_LEN);
                        names[num_names].count = names[j].count;
                        num_names++;
                    }
                }

                // Print final results
                for (int j = 0; j < num_names; j++)
                {
                    printf("%s:%d\n", names[j].name, names[j].count);
                }
            }
            else
            {
                fprintf(stderr, "Child process %d failed\n", pid[i]);
                exit(EXIT_FAILURE);
            }

            close(fd[i][0]); // Close read end of pipe
        }
    }
    return 0;
}