/**
 * Description: Main file of Exercise 8.14 - Contacts from ZyBooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 13, 2023
 * Creation date: April 13, 2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ContactNode.h"

int main() {
    char name[50];
    char phone[50];
    ContactNode* headNode = NULL;
    ContactNode* currentNode = NULL;
    
    // Prompt user for contact 1
    printf("Person 1\n");
    printf("Enter name:\n");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0'; // remove newline character
    printf("Enter phone number:\n");
    fgets(phone, 50, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // remove newline character
    printf("You entered: %s, %s\n", name, phone);
    
    // Create contact 1
    headNode = (ContactNode*) malloc(sizeof(ContactNode));
    CreateContactNode(headNode, name, phone, NULL);
    currentNode = headNode;
    
    // Prompt user for contact 2
    printf("Person 2\n");
    printf("Enter name:\n");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0'; // remove newline character
    printf("Enter phone number:\n");
    fgets(phone, 50, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // remove newline character
    printf("You entered: %s, %s\n", name, phone);
    
    // Create contact 2
    ContactNode* newNode = (ContactNode*) malloc(sizeof(ContactNode));
    CreateContactNode(newNode, name, phone, NULL);
    InsertContactAfter(headNode, newNode);
    currentNode = newNode;
    
    // Prompt user for contact 3
    printf("Person 3\n");
    printf("Enter name:\n");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0'; // remove newline character
    printf("Enter phone number:\n");
    fgets(phone, 50, stdin);
    phone[strcspn(phone, "\n")] = '\0'; // remove newline character
    printf("You entered: %s, %s\n", name, phone);
    
    // Create contact 3
    newNode = (ContactNode*) malloc(sizeof(ContactNode));
    CreateContactNode(newNode, name, phone, NULL);
    InsertContactAfter(currentNode, newNode);
    
    // Print contact list
    printf("\nCONTACT LIST\n");
    currentNode = headNode;
    while (currentNode != NULL) {
        PrintContactNode(currentNode);
        currentNode = GetNextContact(currentNode);
    }
    
    // Free memory
    currentNode = headNode;
    while (currentNode != NULL) {
        headNode = GetNextContact(headNode);
        free(currentNode);
        currentNode = headNode;
    }
    
    return 0;
}

