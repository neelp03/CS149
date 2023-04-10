/**
 * Description: Memory tracer tool - Assingment 4
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 10, 2023
 * Creation date: March 29, 2023
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1000

// Linked list node structure for storing lines
typedef struct node {
    char *line;
    int index;
    struct node *next;
    struct node *tail;
} Node;

// Stack data structure for tracking function calls
typedef struct stack {
    char *functions[1000];
    int top;
} Stack;

// Function declarations
void push(Stack *stack, char *function);
char *pop(Stack *stack);
void printMemory(char *type, char *function, int line, void *ptr, size_t size);
void *safeMalloc(size_t size, char *function, int line);
void *safeRealloc(void *ptr, size_t size, char *function, int line);
void safeFree(void *ptr, char *function, int line);
Node *createNode(char *line, int index);
void addNodeToList(Node **head, char *line, int index);
void printNodes(Node *head);

int main() {
    // Redirect stdout to memtrace.out file
    FILE *fp = fopen("memtrace.out", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    dup2(fileno(fp), STDOUT_FILENO);
    setbuf(stdout, NULL);

    // Initialize stack for tracking function calls
    Stack callStack;
    callStack.top = -1;

    // Initialize array of char * pointers with malloc to an initial size of 10
    char **lines = safeMalloc(10 * sizeof(char *), "main", __LINE__);

    // Read lines from stdin into dynamically allocated array of char *
    char buffer[MAX_LINE_LENGTH];
    int index = 0;
    int arraySize = 10;
    Node *head = NULL;
  
    while (fgets(buffer, MAX_LINE_LENGTH, stdin)) {
        // Remove newline character from end of line
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }

        // Add line to array and linked list
        char *line = safeMalloc((len+1) * sizeof(char), "main", __LINE__);
        strncpy(line, buffer, len+1);
        lines[index] = line;
        addNodeToList(&head, line, index);

        // Check if array needs to be reallocated
        if (index == arraySize-1) {
            arraySize *= 2;
            lines = safeRealloc(lines, arraySize * sizeof(char *), "main", __LINE__);
        }

        index++;
    }

    // Print contents of linked list
    printNodes(head);

    safeFree(lines, "main", __LINE__);
    
    Node *current = head;
	while (current != NULL) {
		Node *temp = current;
		current = current->next;
		safeFree(temp->line, "main", __LINE__);
		safeFree(temp, "main", __LINE__);
	}
    return 0;
}

// Stack function implementations
void push(Stack *stack, char *function) {
    stack->functions[++stack->top] = function;
}

char *pop(Stack *stack) {
    if (stack->top == -1) {
        return NULL;
    }
    return stack->functions[stack->top--];
}

// Memory tracing function implementations
void printMemory(char *type, char *function, int line, void *ptr, size_t size) {
    printf("%s: function=%s, line=%d, ptr=%p, size=%lu\n", type, function, line, ptr, size);
}

void *safeMalloc(size_t size, char *function, int line) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in function %s, line %d\n", function, line);
        exit(1);
    }
    printMemory("MALLOC", function, line, ptr, size);
    return ptr;
}

void *safeRealloc(void *ptr, size_t size, char *function, int line) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed in function %s, line %d\n", function, line);
        exit(1);
    }
    printMemory("REALLOC", function, line, new_ptr, size);
    return new_ptr;
}

void safeFree(void *ptr, char *function, int line) {
    if (ptr != NULL) {
        printMemory("FREE", function, line, ptr, 0);
        free(ptr);
    }
}

Node *createNode(char *line, int index) {
    Node *node = (Node *)safeMalloc(sizeof(Node), "createNode", __LINE__);
    node->line = line;
    node->index = index;
    node->next = NULL;
    node->tail = node;
    return node;
}

void addNodeToList(Node **head, char *line, int index) {
    Node *newNode = createNode(line, index);
    if (*head == NULL) {
        *head = newNode;
        (*head)->tail = newNode; // set the tail pointer to the new node
        return;
    }

    (*head)->tail->next = newNode; // add new node to the end of the list
    (*head)->tail = newNode; // update the tail pointer
}


void printNodes(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d: %s\n", current->index, current->line);
        current = current->next;
    }
}

