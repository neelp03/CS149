/**
 * Description: Assignment 6 - Thread Manager
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: May 10, 2023
 * Creation date: April 25, 2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define HASHSIZE 101
#define MAX_NAMES 100
#define MAX_LENGTH 31

pthread_mutex_t tlock_log = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tlock_data = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tlock_insert = PTHREAD_MUTEX_INITIALIZER;

void* thread_runner(void*);
pthread_t tid1, tid2;
struct THREADDATA_STRUCT {
    pthread_t creator;
};

typedef struct THREADDATA_STRUCT THREADDATA;
THREADDATA* p = NULL;

int logindex=0;
int *logip = &logindex;

/**
 * Function: void logprint(char* message)
 * Description: Prints a log message to the standard output with a timestamp.
 * Parameters:
 *     - message: The message to be logged
 * Returns: None
 **/
void logprint(char* message) {

    int hours, minutes, seconds, day, month, year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    hours = local->tm_hour; 
    minutes = local->tm_min; 
    seconds = local->tm_sec; 
    day = local->tm_mday; 
    month = local->tm_mon + 1; 
    year = local->tm_year + 1900; 

    pthread_mutex_lock(&tlock_log);
    if (hours < 12) 
        fprintf(stdout, "Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d am: %s\n", 
        ++logindex, pthread_self(), getpid(), day, month, year, hours, minutes, seconds, message);
    else 
        fprintf(stdout, "Logindex %d, thread %ld, PID %d, %02d/%02d/%d %02d:%02d:%02d pm: %s\n", 
        ++logindex, pthread_self(), getpid(), day, month, year, hours - 12, minutes, seconds, message);
    pthread_mutex_unlock(&tlock_log);
}

struct node {
    int count;              
    char* name;             
    struct node *next;     
    pthread_mutex_t lock;   
};

static struct node *hashtab[HASHSIZE]; 

/**
 * Function: unsigned hash(char* s)
 * Description: Computes the hash value for a given string.
 * Parameters:
 *     - s: The input string
 * Returns: The computed hash value
 **/
unsigned hash(char* s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }

    return hashval % HASHSIZE;
}

/**
 * Function: struct node* lookup(char* name)
 * Description: Looks up a name in the hash table and returns the corresponding node if found.
 * Parameters:
 *     - name: The name to be looked up
 * Returns: The node containing the name if found, otherwise NULL
 **/
struct node *lookup(char* name)
{
    struct node *np;
    for (np = hashtab[hash(name)]; np != NULL; np = np->next)
        if (strcmp(name, np->name) == 0)
            return np; 
    return NULL; 
}

/**
 * Function: struct node* insert(char* name)
 * Description: Inserts a name into the hash table if it doesn't already exist, or increments the count if it does.
 * Parameters:
 *     - name: The name to be inserted
 * Returns: The inserted or updated node
 **/
struct node *insert(char* name)
{
    struct node *np = NULL;
    unsigned hashval;

    pthread_mutex_lock(&tlock_insert);    
    if ((np = lookup(name)) == NULL) { 
        np = (struct node *) malloc(sizeof(struct node));
        if (np == NULL) { return NULL; }
        np->name = strdup(name);
        np->count = 1;
        pthread_mutex_init(&(np->lock), NULL);
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else {
        pthread_mutex_lock(&(np->lock));
        np->count++;
        pthread_mutex_unlock(&(np->lock));
    }
    pthread_mutex_unlock(&tlock_insert);

    return np;
}

/**
 * Function: struct node* increment(char* name)
 * Description: Increments the count for a given name. If the name doesn't exist, it is inserted into the hash table.
 * Parameters:
 *     - name: The name to be incremented
 * Returns: The node corresponding to the incremented name
 **/
struct node* increment(char* name) {
    struct node* toInc = NULL;
    toInc = lookup(name);

    if(toInc != NULL) {
        pthread_mutex_lock(&(toInc->lock));
        toInc->count++;
        pthread_mutex_unlock(&(toInc->lock));
        return toInc;
    }

    return insert(name);

}

/**
 * Function: void deleteList(struct node* head)
 * Description: Deletes a linked list of nodes, freeing the memory allocated for each node.
 * Parameters:
 *     - head: The head node of the linked list
 * Returns: None
 **/
void deleteList(struct node* head) {
    struct node* current = head;
    while(current != NULL) {
        struct node* temp = current->next;
        free(current->name);
        free(current);
        current = temp;
    }
}

/**
 * Function: void printList(struct node* head)
 * Description: Prints the contents of a linked list of nodes.
 * Parameters:
 *     - head: The head node of the linked list
 * Returns: None
 **/
void printList(struct node* head) {
    struct node* current = head;
    while(current != NULL) {
        fprintf(stdout,"%s. %d\n",current->name,current->count);
        current = current->next;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
        fprintf(stderr, "need 2 files\n");
        pthread_exit(NULL);
    }
    printf("-------- LOG MESSAGES ---------");
    printf("created thread 1\n");
    pthread_create(&tid1,NULL,thread_runner,argv[1]);

    printf("create thread 2\n");
    pthread_create(&tid2,NULL,thread_runner,argv[2]);

    printf("waiting for thread 1\n");
    pthread_join(tid1,NULL);
    printf("thread 1 finished\n");

    printf("waiting for thread 2\n");
    pthread_join(tid2,NULL);
    printf("thread 2 finished\n");

    printf("-------- Name Counts ---------");
    for(int j = 0; j < HASHSIZE; j++) {
        printList(hashtab[j]);
        deleteList(hashtab[j]);
        hashtab[j] = NULL;
    }

    exit(0);
}

/**
 * Function: void* thread_runner(void*)
 * Description: Entry point for the threads. Reads the input file and increments the count for each name found in the file.
 * Parameters:
 *     - f: The input file name (passed as a void pointer)
 * Returns: None
 **/
void* thread_runner(void* f)
{
    char* filename = (char *) f;
    pthread_t my_thread;
    char buffer[100];
    my_thread = pthread_self();
    sprintf(buffer, "This is thread %ld (p=%p)",my_thread,p);
    logprint(buffer);

    pthread_mutex_lock(&tlock_data); 
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator = my_thread;
    }

    if (p != NULL && p->creator == my_thread) {
        sprintf(buffer, "Thread: %ld; THREADDATA: %p",my_thread,p);
        logprint(buffer);
    } else {
        sprintf(buffer, "Thread: %ld; THREADDATA: %p",my_thread,p);
        logprint(buffer);
    }
    pthread_mutex_unlock(&tlock_data); 

    FILE* inFile = fopen(filename, "r");
    logprint(buffer);

    if(!inFile) {
        logprint(buffer);       
        pthread_exit(NULL);
    }

    int i = 0;

    while(true) {
        i++;

        char* input = NULL;
        size_t length;

        if(getline(&input, &length, inFile) == -1) {
            free(input);
            break;
        }

        if(input[0] == '\n' || (input[0] == ' ' && (input[1] == '\n' || input[1] == '\0'))) {
            fprintf(stderr, "Warning - file %s line %d is empty.\n", filename, i);
            free(input);
            continue;
        }

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        while(increment(input) == NULL) {}
        free(input);
    }

    if(ferror(inFile))
    {
        pthread_exit(NULL);
    }

    fclose(inFile);

    pthread_mutex_lock(&tlock_data);
    if (p!=NULL && p->creator==my_thread) {
        sprintf(buffer, "Thread: %ld; THREADDATA: %p",my_thread, p);
        logprint(buffer);     
        free(p);
        p = NULL;
    } else {
        sprintf(buffer, "Thread: %ld; THREADDATA: %p",my_thread, p);
        logprint(buffer);    
    }
    pthread_mutex_unlock(&tlock_data);

    pthread_exit(NULL);

}
