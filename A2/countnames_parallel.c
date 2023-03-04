#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_NAME_LEN 50

struct NameCount
{
    char name[MAX_NAME_LEN];
    int count;
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s file1 file2 ... file%d\n", argv[0], MAX_FILES);
        exit(EXIT_FAILURE);
    }

    int num_files = argc - 1;
    char *files[MAX_FILES];
    for (int i = 0; i < num_files; i++)
    {
        files[i] = argv[i + 1];
    }

    struct NameCount names[MAX_FILES * MAX_NAME_LEN];
    int num_names = 0;

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_files; i++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {                 // child process
            close(fd[0]); // close read end of pipe

            FILE *file = fopen(files[i], "r");
            if (file == NULL)
            {
                perror(files[i]);
                exit(EXIT_FAILURE);
            }

            char name[MAX_NAME_LEN];
            while (fscanf(file, "%s", name) == 1)
            {
                int found = 0;
                for (int j = 0; j < num_names; j++)
                {
                    if (strcmp(name, names[j].name) == 0)
                    {
                        names[j].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    strncpy(names[num_names].name, name, MAX_NAME_LEN);
                    names[num_names].count = 1;
                    num_names++;
                }
            }

            fclose(file);

            if (write(fd[1], names, num_names * sizeof(struct NameCount)) == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
        else
        {                 // parent process
            close(fd[1]); // close write end of pipe

            struct NameCount child_names[MAX_FILES * MAX_NAME_LEN];
            ssize_t num_read = read(fd[0], child_names, sizeof(child_names));

            if (num_read == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            {
                for (int j = 0; j < num_names; j++)
                {
                    int found = 0;
                    for (int k = 0; k < num_read / sizeof(struct NameCount); k++)
                    {
                        if (strcmp(child_names[k].name, names[j].name) == 0)
                        {
                            names[j].count += child_names[k].count;
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                    {
                        strncpy(names[num_names].name, child_names[j].name, MAX_NAME_LEN);
                        names[num_names].count = child_names[j].count;
                        num_names++;
                    }
                }
            }
        }
    }
    for (int i = 0; i < num_names; i++)
    {
        printf("%s:%d\n", names[i], counts[i]);
    }
    return 0;
}
