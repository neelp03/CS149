/**
 * Description: Assignment 5
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 25, 2023
 * Creation date: April 21, 2023
 **/


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_LENGTH 31

struct nlist {
    time_t start;   
    time_t finish;  
    int index;          
    int pid;            
    char *command;      
    struct nlist *next;
};

#define HASHSIZE 101
static struct nlist *hashtable[HASHSIZE]; 

unsigned hash(int pid)
{
    unsigned hashvalue = pid;
    return hashvalue % HASHSIZE;
}

struct nlist *lookup(int pid)
{
    struct nlist *np;
    for (np = hashtable[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
            return np;
    return NULL;
}

struct nlist *insert(int pid)
{
    struct nlist *np;
    unsigned hashvalue;

    if ((np = lookup(pid)) == NULL) { 
        np = (struct nlist *) malloc(sizeof(struct nlist));
        if (np == NULL)
            return NULL;
        np->pid = pid;
        hashvalue = hash(pid);
        np->next = hashtable[hashvalue];
        hashtable[hashvalue] = np;
    }

    return np;
}

int handle_command(char **commands, int index) {
    char filename[MAX_LENGTH];


    sprintf(filename, "%d.out", getpid());
    int fd_out = open(filename, O_RDWR | O_CREAT | O_APPEND, 0777);
    FILE* out = fopen(filename, "a");
    sprintf(filename, "%d.err", getpid());
    int fd_err = open(filename, O_RDWR | O_CREAT | O_APPEND, 0777);
    FILE* err = fopen(filename, "a");

    fprintf(out, "Starting command %d: child %d pid of parent %d\n", index, getpid(), getppid());
    fflush(out);
    char* command = strdup(commands[index]);
    
    int length = strlen(command);
    char **args = (char **)calloc(length, sizeof(char *));
    int j = 0;
    args[j++] = strtok(command, " ");

    while((args[j++] = strtok(NULL, " ")) != NULL) { }

    execvp(args[0], args);
    fprintf(err, "Could not execute: %s\n", commands[index]);
    return 2;
}

int main( int argc, char *argv[] ) {
    
    memset(hashtable,0,HASHSIZE*sizeof(struct nlist*));

    int pid;

    int count = 0;
    int size= 10;

    char** commands;
    commands = (char**) malloc(size * sizeof(char*));
    memset(commands, 0, size * sizeof(char*));

    char fileName[MAX_LENGTH];

    while(1) {

        if(count >= size) {
            size += 10;
            commands = (char**) realloc(commands, size*sizeof(char *));
        }

        char* input = (char*) malloc(MAX_LENGTH * sizeof(char));
        memset(input, 0, MAX_LENGTH * sizeof(char));

        if( fgets(input, MAX_LENGTH, stdin) == NULL) {
            free(input);
            break;
        }

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        commands[count] = input;
        count++;  
    }
	
	int i = 0;
	while(i < count) {
        time_t start = time(NULL);
        pid = fork();
        
        if(pid < 0) {
            return 1;
        }

        else if(pid == 0) {
            break;
        }
        else {
            struct nlist* hashnode = insert(pid);
            hashnode->command = commands[i];
            hashnode->index = ++i;
            hashnode->start = start;
        }
    }

    if(pid > 0) {
        int pid, status;
        
        while ((pid = wait(&status)) > 0) {
            time_t end = time(NULL);
            struct nlist* node = lookup(pid);
            node->finish = end;

            sprintf(fileName, "%d.out", pid);
            FILE* fd_out = fopen(fileName, "a");
            sprintf(fileName, "%d.err", pid);
            FILE* fd_err = fopen(fileName, "a");

            if (WIFEXITED(status)) {
                fprintf(fd_err, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                fprintf(fd_err, "Killed with signal %d\n", WTERMSIG(status));  
            }

            fprintf(fd_err, "spawning too fast\n"); 
            fprintf(fd_out, "Finished child %d pid of parent %d\n", pid, getpid());
            fprintf(fd_out, "Finished at %ld, runtime duration %ld\n", node->finish, node->finish-node->start);
            fclose(fd_out);
            fclose(fd_err);


            if(node->finish - node->start > 2) {
                time_t start = time(NULL);
                int pid2 = fork();
                if(pid2 < 0) { return 1; }
                if(pid2 == 0) {
                    sprintf(fileName, "%d.out", getpid());
                    FILE* fd_out = fopen(fileName, "a");
                    sprintf(fileName, "%d.err", getpid());
                    FILE* fd_err = fopen(fileName, "a");

                    fprintf(fd_err, "RESTARTING\n"); 
                    fprintf(fd_out, "RESTARTING\n");
                    fclose(fd_out);
                    fclose(fd_err);
					return handle_command(commands, node->index);
                }
                else {
                    struct nlist* tempNode = insert(pid2);
                    tempNode->command = node->command;
                    tempNode->index = node->index;
                    tempNode->start = start;
                }
            }
        }

        for(int j = 0; j < count; j++) {
            free(commands[j]);
        }
        free(commands);

        for(int j = 0; j < HASHSIZE; j++) {
            struct nlist* tempNode = hashtable[j];
            while(tempNode != NULL) {
                struct nlist* nextNode = tempNode->next;
                free(tempNode);
                tempNode = nextNode;
            }
            hashtable[j] = NULL;
        }

    }

    else {
        return handle_command(commands, i);
    }

    return 0;
}
