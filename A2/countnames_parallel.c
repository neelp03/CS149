#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_NAME_LEN 100
#define MAX_FILES 10

struct name_count {
    char name[MAX_NAME_LEN];
    int count;
};

int main(int argc, char *argv[]) {
    int num_files = argc - 1;
    if (num_files > MAX_FILES) {
        fprintf(stderr, "Error: too many files (maximum %d)\n", MAX_FILES);
        exit(1);
    }

    int pipes[num_files][2];
    pid_t pids[num_files];

    // Create pipes and fork child processes
    for (int i = 0; i < num_files; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            close(pipes[i][0]);
            FILE *file = fopen(argv[i+1], "r");
            if (file == NULL) {
                perror("fopen");
                exit(1);
            }
            struct name_count counts[MAX_NAME_LEN] = {0};
            char line[MAX_NAME_LEN];
            while (fgets(line, sizeof(line), file) != NULL) {
    char *first_name = strtok(line, " \t\r\n");
    char *last_name = strtok(NULL, " \t\r\n");
    while (first_name != NULL && last_name != NULL) {
        char name[MAX_NAME_LEN];
        snprintf(name, MAX_NAME_LEN, "%s %s", first_name, last_name);
        int found = 0;
        for (int j = 0; j < MAX_NAME_LEN; j++) {
            if (counts[j].count == 0) {
                strcpy(counts[j].name, name);
                counts[j].count = 1;
                found = 1;
                break;
            } else if (strcmp(counts[j].name, name) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Error: too many names\n");
            exit(1);
        }
        first_name = strtok(NULL, " \t\r\n");
        last_name = strtok(NULL, " \t\r\n");
    }
}


            fclose(file);
            for (int j = 0; j < MAX_NAME_LEN; j++) {
                if (counts[j].count > 0) {
                    char buf[MAX_NAME_LEN+10];
                    sprintf(buf, "%s: %d", counts[j].name, counts[j].count);
                    if (write(pipes[i][1], buf, sizeof(buf)) == -1) {
                        perror("write");
                        exit(1);
                    }
                }
            }
            close(pipes[i][1]);
            exit(0);
        } else {
            // Parent process
            pids[i] = pid;
            close(pipes[i][1]);
        }
    }

    // Read name counts from child processes and combine them
    struct name_count combined_counts[MAX_NAME_LEN] = {0};
    for (int i = 0; i < num_files; i++) {
        char buf[MAX_NAME_LEN+10];
        while (read(pipes[i][0], buf, sizeof(buf)) > 0) {
            char *name = strtok(buf, ":");
            char *count_str = strtok(NULL, ":");
                    int count = atoi(count_str);
        int found = 0;
        for (int j = 0; j < MAX_NAME_LEN; j++) {
            if (combined_counts[j].count == 0) {
                strcpy(combined_counts[j].name, name);
                combined_counts[j].count = count;
                found = 1;
                break;
            } else if (strcmp(combined_counts[j].name, name) == 0) {
                combined_counts[j].count += count;
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Error: too many names\n");
            exit(1);
        }
    }
    close(pipes[i][0]);
}

// Wait for child processes to finish
for (int i = 0; i < num_files; i++) {
    int status;
    if (waitpid(pids[i], &status, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "Error: child process exited with status %d\n", status);
        exit(1);
    }
}

// Output combined name counts
for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (combined_counts[i].count > 0) {
        printf("%s: %d\n", combined_counts[i].name, combined_counts[i].count);
    }
}

return 0;
}

