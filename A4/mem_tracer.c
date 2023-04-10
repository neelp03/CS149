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
#include <stdarg.h>

/**
//CS149 assignment#4 helper code.
// See the TODO's in the comments below! You need to implement those.
**/

/**
*/ TRACE_NODE_STRUCT is a linked list of
// pointers to function identifiers
// TRACE_TOP is the head of the list is the top of the stack

struct TRACE_NODE_STRUCT {
char functionid; // ptr to function identifier (a function name)
struct TRACE_NODE_STRUCT next; // ptr to next frama
};
typedef struct TRACE_NODE_STRUCT TRACE_NODE;
static TRACE_NODE TRACE_TOP = NULL; // ptr to the top of the stack

/* --------------------------------/
/ function PUSH_TRACE /

The purpose of this stack is to trace the sequence of function calls,
just like the stack in your computer would do.
The "global" string denotes the start of the function call trace.
The char *p parameter is the name of the new function that is added to the call trace.
See the examples of calling PUSH_TRACE and POP_TRACE below
in the main, make_extend_array, add_column functions.
*/
void PUSH_TRACE(char p) // push p on the stack
{
TRACE_NODE tnode;
static char glob[]="global";
if (TRACE_TOP==NULL) {
// initialize the stack with "global" identifier
TRACE_TOP=(TRACE_NODE*) malloc(sizeof(TRACE_NODE));

// no recovery needed if allocation failed, this is only
// used in debugging, not in production
if (TRACE_TOP==NULL) {
  printf("PUSH_TRACE: memory allocation error\n");
  exit(1);
}

TRACE_TOP->functionid = glob;
TRACE_TOP->next=NULL;
}//if

// create the node for p
tnode = (TRACE_NODE*) malloc(sizeof(TRACE_NODE));

// no recovery needed if allocation failed, this is only
// used in debugging, not in production
if (tnode==NULL) {
printf("PUSH_TRACE: memory allocation error\n");
exit(1);
}//if

tnode->functionid=p;
tnode->next = TRACE_TOP; // insert fnode as the first in the list
TRACE_TOP=tnode; // point TRACE_TOP to the first node

}
//end PUSH_TRACE/

/* --------------------------------/
/ function POP_TRACE /
/ Pop a function call from the stack /
*/
void POP_TRACE() // remove the op of the stack
{
TRACE_NODE tnode;
tnode = TRACE_TOP;
TRACE_TOP = tnode->next;
free(tnode);

}
// end POP_TRACE

/* ---------------------------------------------- /
/ function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance /
/ For example, it returns a string that looks like: global:funcA:funcB:funcC. /
/ Printing the function call sequence the other way around is also ok: funcC:funcB:funcA:global /
*/
/* ---------------------------------------------- */
/* function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance */
/* For example, it returns a string that looks like: global:funcA:funcB:funcC. */
/* Printing the function call sequence the other way around is also ok: funcC:funcB:funcA:global */

char* PRINT_TRACE()
{
  int depth = 50; // A max of 50 levels in the stack will be combined in a string for printing out.
  int i, length, j;
  TRACE_NODE* tnode;
  static char buf[100];

  if (TRACE_TOP == NULL) {     // stack not initialized yet, so we are
    strcpy(buf, "global");     // still in the `global' area
    return buf;
  }

  /* Peek at the top depth (50) entries on the stack, but do not
     go over 100 chars and do not go over the bottom of the stack */
  sprintf(buf, "%s", TRACE_TOP->functionid);
  length = strlen(buf);        // length of the string so far
  for (i = 1, tnode = TRACE_TOP->next; tnode != NULL && i < depth; i++, tnode = tnode->next) {
    j = strlen(tnode->functionid);    // length of what we want to add
    if (length + j + 1 < 100) {       // total length is ok
      sprintf(buf + length, ":%s", tnode->functionid);
      length += j + 1;
    } else {                          // it would be too long
      break;
    }
  }

  // Print info about memory usage
  printf("File %s, line %d, function %s ", __FILE__, __LINE__, __func__);
  printf("allocated memory segment at address %p to size %d\n", buf, length);
  printf("Current call trace: %s\n", buf);

  return buf;
} /* end PRINT_TRACE */


