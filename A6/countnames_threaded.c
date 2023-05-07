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

unsigned hash(char* s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }

    return hashval % HASHSIZE;
}

struct node *lookup(char* name)
{
    struct node *np;
    for (np = hashtab[hash(name)]; np != NULL; np = np->next)
        if (strcmp(name, np->name) == 0)
            return np; 
    return NULL; 
}

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

void deleteList(struct node* head) {
    struct node* current = head;
    while(current != NULL) {
        struct node* temp = current->next;
        free(current->name);
        free(current);
        current = temp;
    }
}

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
        fprintf(stderr, "Program requires exactly two file names.\n");
        pthread_exit(NULL);
    }

    printf("==================== Log Messages ====================\n");

    printf("create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,argv[1]);

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,argv[2]);

    printf("wait for first thread to exit\n");
    pthread_join(tid1,NULL);
    printf("first thread exited\n");

    printf("wait for second thread to exit\n");
    pthread_join(tid2,NULL);
    printf("second thread exited\n");

    printf("==================== Name Counts ====================\n");

    for(int j = 0; j < HASHSIZE; j++) {
        printList(hashtab[j]);
        deleteList(hashtab[j]);
        hashtab[j] = NULL;
    }

    exit(0);
}

void* thread_runner(void* f)
{
    char* filename = (char *) f;
    pthread_t me;
    char buffer[100];
    me = pthread_self();
    sprintf(buffer, "This is thread %ld (p=%p)",me,p);
    logprint(buffer);

    pthread_mutex_lock(&tlock_data); 
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator = me;
    }

    if (p != NULL && p->creator == me) {
        sprintf(buffer, "This is thread %ld and I created THREADDATA %p",me,p);
        logprint(buffer);
    } else {
        sprintf(buffer, "This is thread %ld and I can access the THREADDATA %p",me,p);
        logprint(buffer);
    }
    pthread_mutex_unlock(&tlock_data); 

    FILE* inFile = fopen(filename, "r");
    sprintf(buffer, "opened file %s", filename);
    logprint(buffer);

    if(!inFile) {
        sprintf(buffer, "range: cannot open file\n");
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
    if (p!=NULL && p->creator==me) {
        sprintf(buffer, "This is thread %ld and I delete THREADDATA",me);
        logprint(buffer);     
        free(p);
        p = NULL;
    } else {
        sprintf(buffer, "This is thread %ld and I can access the THREADDATA",me);
        logprint(buffer);    
    }
    pthread_mutex_unlock(&tlock_data);

    pthread_exit(NULL);

}
