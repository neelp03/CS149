#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAME_LEN 100

int main(int argc, char *argv[]) {
    int num_files = argc - 1;
    int pipes[num_files][2];

    for (int i = 0; i < num_files; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process reads from file and writes to pipe
            close(pipes[i][0]);

            FILE *fp = fopen(argv[i + 1], "r");
            if (fp == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }

            char name[MAX_NAME_LEN];
            int count = 0;
            while (fscanf(fp, "%s", name) != EOF) {
                count++;
            }

            fprintf(stderr, "Child %d counting %d names in file %s\n", getpid(), count, argv[i + 1]);

            dprintf(pipes[i][1], "%s:%d\n", argv[i + 1], count);

            fclose(fp);
            close(pipes[i][1]);

            exit(EXIT_SUCCESS);
        } else {
            // Parent process closes write end of pipe
            close(pipes[i][1]);
        }
    }

    // Parent process reads from pipes and prints output
    for (int i = 0; i < num_files; i++) {
        char buf[MAX_NAME_LEN + 10];
        int nbytes = read(pipes[i][0], buf, MAX_NAME_LEN + 10);
        if (nbytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("%.*s", nbytes, buf);

        close(pipes[i][0]);
    }

    // Parent process waits for child processes to complete
    for (int i = 0; i < num_files; i++) {
        wait(NULL);
    }

    return 0;
}
