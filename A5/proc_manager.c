/**
 * Description: Assignment 5
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 22, 2023
 * Creation date: April 21, 2023
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define HASHSIZE 101
#define MAXLINE 1000
#define MAXARGS 128
#define MAXCHILDREN 50
#define MAXRUNTIME 2

static struct nlist *hashtab[HASHSIZE];

struct nlist {
    struct nlist *next; /* next entry in chain */
    int pid;  /* process ID */
    char *command;  /* command to execute */
    int index; /* line index in input file */
    time_t starttime; /* start time */
    time_t finishtime; /* finish time */
};

/**
This function calculates a hash value based on the given process ID
Assumption: none
Input parameters: an integer representing the process ID
Returns: an unsigned integer representing the hash value
**/
unsigned hash(int pid)
{
    return pid % HASHSIZE;
}

/**
This function searches the hash table for an entry with the given process ID
Assumption: none
Input parameters: an integer representing the process ID
Returns: a pointer to the entry if found, NULL if not found
**/
struct nlist *lookup(int pid)
{
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
          return np; /* found */
    return NULL; /* not found */
}


/**
This function inserts a new entry into the hash table with the given process ID, command, line index, and start time
Assumption: none
Input parameters: an integer representing the process ID, a string representing the command, an integer representing the line index, and a time_t representing the start time
Returns: a pointer to the new entry if successful, NULL if unsuccessful
**/
struct nlist *insert(int pid, char *command, int index, time_t start)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(pid)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->command = strdup(command)) == NULL)
            return NULL;
        hashval = hash(pid);
        np->pid = pid;
        np->index = index;
        np->starttime = start;
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    return np;
}

/**
This function spawns a new child process to execute the given command
Assumption: none
Input parameters: an array of strings representing the command and its arguments, an integer representing the line index, and a time_t representing the start time
Returns: the process ID of the new child process if successful, -1 if unsuccessful
**/
int spawn_child(char *argv[], int index, time_t start)
{
    pid_t pid;
    int stdout_fd, stderr_fd;

    if ((pid = fork()) < 0) {
        printf("fork error: %s\n", strerror(errno));
        return -1;
    } else if (pid == 0) {
        /* child process */
        char out_file[MAXLINE], err_file[MAXLINE];
        sprintf(out_file, "%d.out", getpid());
        sprintf(err_file, "%d.err", getpid());
        stdout_fd = dup(STDOUT_FILENO);
        stderr_fd = dup(STDERR_FILENO);
        freopen(out_file, "w", stdout);
        freopen(err_file, "w", stderr);
        printf("Started child process with pid %d at %ld\n", pid, start);
        execvp(argv[0], argv);
        /* if execvp returns, there was an error */
        printf("execvp error: %s\n", strerror(errno));
        exit(1);
    }
    /* parent process */
    if (insert(pid, argv[0], index, start) == NULL) {
        printf("insert error: %s\n", strerror(errno));
        return -1;
    }
    return pid;
}




/**

This function handles the SIGCHLD signal and cleans up the finished child processes
Assumption: none
Input parameters: an integer representing the signal number
Returns: nothing
**/
void handle_signal(int sig)
{
    pid_t pid;
    time_t finish;
    int status;
    struct nlist *np;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        np = lookup(pid);
        if (np != NULL) {
            finish = time(NULL);
            np->finishtime = finish;
            if (WIFEXITED(status))
                printf("Finished child %d pid of parent %d\n", pid, getpid());
            else
                printf("Exited with exitcode = %d\n", WEXITSTATUS(status));
            printf("Finished at %ld, runtime duration %ld\n", finish, finish - np->starttime);
            if (finish - np->starttime > MAXRUNTIME) {
                printf("Maximum runtime exceeded for child %d pid of parent %d\n", pid, getpid());
                kill(pid, SIGKILL);
            }	
            free(np->command);
            np->next = NULL;
            free(np);
        }
    }
}


/**
This is the main function that reads commands from the user and spawns child processes to execute them
Assumption: the user provides valid commands and arguments
Input parameters: command line arguments
Returns: an integer
**/
int main(int argc, char *argv[])
{
    char *line, *cmd;
    char *args[MAXARGS];
    int i, pid;
    size_t len = 0;
    ssize_t nread;
    struct sigaction sa;

    /* Set up signal handler for SIGCHLD */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        printf("sigaction error: %s\n", strerror(errno));
        exit(1);
    }

    while (1) {
        printf("> ");
        fflush(stdout);

        /* Read a line from stdin */
        if ((nread = getline(&line, &len, stdin)) == -1) {
            if (errno == EINTR)
                continue;
            else
                break;
        }

        /* Parse the line into a command and its arguments */
        cmd = strtok(line, " \n");
        for (i = 0; i < MAXARGS - 1 && cmd != NULL; i++) {
            args[i] = cmd;
            cmd = strtok(NULL, " \n");
        }
        args[i] = NULL;

        /* Spawn a child process to execute the command */
        if ((pid = spawn_child(args, i, time(NULL))) < 0) {
            printf("spawn_child error: %s\n", strerror(errno));
            exit(1);
        }

        /* Redirect output and error to <pid>.out and <pid>.err files */
        char out_file[MAXLINE], err_file[MAXLINE];
        sprintf(out_file, "%d.out", pid);
        sprintf(err_file, "%d.err", pid);
        int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        int err_fd = open(err_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (out_fd < 0 || err_fd < 0) {
            printf("open error: %s\n", strerror(errno));
            exit(1);
        }
        dup2(out_fd, STDOUT_FILENO);
        dup2(err_fd, STDERR_FILENO);
        close(out_fd);
        close(err_fd);
    }

    /* Clean up */
    free(line);
    for (i = 0; i < HASHSIZE; i++) {
        struct nlist *np = hashtab[i];
        while (np != NULL) {
            struct nlist *next = np->next;
            free(np->command);
            free(np);
            np = next;
        }
    }

    return 0;
}
