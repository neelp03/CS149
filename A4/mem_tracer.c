#include <stdio.h>   // for printf, FILE, fopen, fclose, fprintf
#include <stdlib.h>  // for malloc, free, exit
#include <string.h>  // for strcmp, strlen, strncpy
#include <fcntl.h>   // for open, O_CREAT, O_WRONLY, O_TRUNC
#include <unistd.h>  // for write
#include <sys/stat.h> // for S_IRUSR, S_IWUSR


// Node structure for linked list
typedef struct node {
    char* line;
    int index;
    struct node* next;
    char* function;
} node_t;

// Stack structure for function calls
typedef struct stack {
    int top;
    char** function;
    int size;
} stack_t;

// Function prototypes
char** make_extend_array(char** array, int size, int increment, int* count);
node_t* add_node(node_t* head, char* line, int index);
void print_nodes(node_t* head);
void push(stack_t* stack, char* function);
char* pop(stack_t* stack);
void mem_alloc(char* function, int size);
void mem_realloc(char* function, void* ptr, int size);
void mem_free(char* function, void* ptr);

int main() {
    // Redirect stdout to memtrace.out
    int fd = open("memtrace.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    dup2(fd, STDOUT_FILENO);

    char** array = NULL;
    int size = 10;
    int increment = 10;
    int count = 0;

    // Initialize stack for memory tracing
    stack_t stack;
    stack.top = -1;
    stack.function = malloc(sizeof(char*) * 100);

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    node_t* head = NULL;
    int index = 0;

    // Read lines from stdin
    while ((read = getline(&line, &len, stdin)) != -1) {
        // Remove newline character at end of line
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        // Store line in array
        if (count >= size) {
            array = make_extend_array(array, size, increment, &count);
            push(&stack, "make_extend_array");
        }
        array[count] = malloc(strlen(line) + 1);
        push(&stack, "malloc");
        strcpy(array[count], line);
        count++;

        // Store line in linked list
        head = add_node(head, line, index);
        push(&stack, "add_node");
        index++;
    }

    // Print content of linked list
    print_nodes(head);
    push(&stack, "print_nodes");

    // Free memory allocated for linked list
    node_t* current = head;
    while (current != NULL) {
        node_t* next = current->next;
        free(current->line);
        push(&stack, "free");
        free(current);
        push(&stack, "free");
        current = next;
    }

    // Free memory allocated for array
    for (int i = 0; i < count; i++) {
        free(array[i]);
        push(&stack, "free");
    }
    free(array);
    push(&stack, "free");

    // Free memory allocated for stack
    free(stack.function);
    push(&stack, "free");

    return 0;
}

// Function to allocate and reallocate memory for array
char** make_extend_array(char** array, int size, int increment, int* count) {
    // Print memory allocation message
    mem_alloc("make_extend_array", sizeof(char*) * (size + increment));

    // Reallocate memory for array
    array = realloc(array, sizeof(char*) * (size + increment));

    // Print memory reallocation message
	mem_realloc("make_extend_array", (void**) &array, sizeof(char*) * (size + increment));

	// Check if reallocation was successful
	if (array == NULL) {
		printf("Error: could not reallocate memory for array\n");
		exit(1);
	}

	// Increment count by the number of new elements added to array
	*count += increment;

	// Return the updated array
	return array;
}

node_t* add_node(node_t* head, char* line, int index) {
    // Allocate memory for new node
    node_t* new_node = (node_t*)malloc(sizeof(node_t));

    // Initialize new node
    new_node->line = line;
    new_node->index = index;
    new_node->next = NULL;

    // If list is empty, new node is head
    if (head == NULL) {
        head = new_node;
    }
    // Otherwise, add new node to end of list
    else {
        node_t* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    // Return updated head
    return head;
}

void print_nodes(node_t* head) {
    node_t* current = head;

    while (current != NULL) {
        printf("%p\n", current->function);
        current = current->next;
    }
}

void push(stack_t* stack, char* function) {
    // Create new node for function
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    new_node->function = function;
    new_node->next = NULL;

    // Add new node to top of stack
    new_node->next = (node_t*)stack->top;
	stack->top = (void*)new_node;

    stack->size++;
}

// Function to remove the top element from the stack and return it
void* pop(stack_t* stack) {
    // Check if stack is empty
    if (stack->top != NULL) {
        printf("Error: stack is empty\n");
        return NULL;
    }

    // Store the top element and remove it from the stack
    void (*function)() = ((node_t*)stack->top)->function;

    int* temp = (int*)stack->top;
    stack->top = (node_t *)stack->top->next;
    free(temp);

    return function;
}

// Function to print memory allocation message
void mem_alloc(char* function, int size) {
    printf("Memory allocated in function %s, size = %d\n", function, size);
}

void mem_realloc(char* function, void* ptr, int size) {
    // Print memory reallocation message
    printf("file-\"%s\", function=%s, segment reallocated to address %p\n", __FILE__, function, ptr);
    
    // Reallocate memory
    ptr = realloc(ptr, size);
    
    // Check if reallocation failed
    if (ptr == NULL) {
        fprintf(stderr, "Memory reallocation failed in function %s\n", function);
        exit(EXIT_FAILURE);
    }
}

void mem_free(char* function, void* ptr) {
    // Print memory deallocation message
    printf("file-\"%s\", function=%s, segment freed at address %p\n", __FILE__, function, ptr);
    
    // Free memory
    free(ptr);
}


