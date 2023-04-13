/**
 * Description: ContactNode file from Exercise 8.14 - Contacts from ZyBooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 13, 2023
 * Creation date: April 13, 2023
 **/
 
 #include "ContactNode.h"
#include <stdio.h>
#include <string.h>

void CreateContactNode(ContactNode* thisNode, char name[], char phoneNum[], ContactNode* nextLoc) {
    strcpy(thisNode->contactName, name);
    strcpy(thisNode->contactPhoneNum, phoneNum);
    thisNode->nextNodePtr = nextLoc;
}

void InsertContactAfter(ContactNode* thisNode, ContactNode* newNode) {
    ContactNode* tmpNext = thisNode->nextNodePtr;
    thisNode->nextNodePtr = newNode;
    newNode->nextNodePtr = tmpNext;
}

ContactNode* GetNextContact(ContactNode* thisNode) {
    return thisNode->nextNodePtr;
}

void PrintContactNode(ContactNode* thisNode) {
    printf("Name: %s\n", thisNode->contactName);
    printf("Phone number: %s\n", thisNode->contactPhoneNum);
}

