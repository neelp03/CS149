/**
 * Description: Assignment 4
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 5, 2023
 * Creation date: March 29, 2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define MAX_LINE_LENGTH 1024

// Structure to represent a node in the linked list
struct Node {
  char* line;
  int index;
  struct Node* next;
};
typedef struct Node Node;

// Global variables for memory tracing
FILE* mem_trace_file = NULL;
int total_memory_allocated = 0;
int total_memory_freed = 0;

// Function prototypes
void push_trace(char* function_name);
void pop_trace();
void* my_malloc(size_t size, char* function_name);
void* my_realloc(void* ptr, size_t size, char* function_name);
void my_free(void* ptr, char* function_name);
void print_nodes(Node* head);
void cleanup(Node* head);

int main() {
  // Redirect stdout to memtrace.out
  mem_trace_file = fopen("memtrace.out", "w");
  if (mem_trace_file == NULL) {
    printf("Failed to open memtrace.out for writing\n");
    exit(1);
  }
  dup2(fileno(mem_trace_file), STDOUT_FILENO);

  // Initialize the array to store command lines
  char** array = (char**)my_malloc(INITIAL_SIZE * sizeof(char*), "main");
  int array_size = INITIAL_SIZE;
  int array_index = 0;

  // Initialize the linked list to store lines
  Node* head = NULL;
  Node* tail = NULL;

  // Read lines from stdin
  char buffer[MAX_LINE_LENGTH];
  int line_index = 0;
  
  while (fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL) {
    // todo
  }
  
  // Todo:
  // Print the nodes in the linked list
  // Cleanup and free memory

  return 0;
}

void push_trace(char* function_name) {
  TRACE_NODE* tnode = (TRACE_NODE*)my_malloc(sizeof(TRACE_NODE), function_name);
  tnode->functionid = function_name;
  tnode->next = TRACE_TOP;
  TRACE_TOP = tnode;
}

void pop_trace() {
  TRACE_NODE* tnode = TRACE_TOP;
  TRACE_TOP = TRACE_TOP->next;
  my_free(tnode, "pop_trace");
}

void* my_malloc(size_t size, char* function_name) {
  // Push the function name to the memory trace stack
  push_trace(function_name);

  // Allocate memory using malloc
  void* ptr = malloc(size);

  // Update total memory allocated and print memory trace
  total_memory_allocated += size;
  fprintf(mem_trace_file, "Allocated %lu bytes at address %p in function %s\n", size, ptr, function_name);

  // Pop the function name from the memory trace stack
  pop_trace();

  return ptr;
}

void* my_realloc(void* ptr, size_t size, char* function_name) {
  // Push the function name to the memory trace stack
  push_trace(function_name);

  // Reallocate memory using realloc
  void* new_ptr = realloc(ptr, size);

  // Update total memory allocated and freed and print memory trace
  if (new_ptr != NULL) {
    int old_size = malloc_usable_size(ptr);
    total_memory_allocated += size - old_size;
    total_memory_freed += old_size;
    fprintf(mem_trace_file, "Reallocated %lu bytes at address %p in function %s\n", size, new_ptr, function_name);
  } else {
    fprintf(mem_trace_file, "Failed to reallocate %lu bytes in function %s\n", size, function_name);
  }

  // Pop the function name from the memory trace stack
  pop_trace();

  return new_ptr;
}

