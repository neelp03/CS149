/**
 * Description: Header file for main file of Exercise 8.14 - Contacts from ZyBooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 13, 2023
 * Creation date: April 13, 2023
 **/

#ifndef CONTACT_NODE_H
#define CONTACT_NODE_H

#define MAX_NAME_LENGTH 50
#define MAX_PHONE_NUM_LENGTH 50

typedef struct ContactNode {
    char contactName[MAX_NAME_LENGTH];
    char contactPhoneNum[MAX_PHONE_NUM_LENGTH];
    struct ContactNode* nextNodePtr;
} ContactNode;

void CreateContactNode(ContactNode* thisNode, char name[], char phoneNum[], ContactNode* nextLoc);
void InsertContactAfter(ContactNode* thisNode, ContactNode* newNode);
ContactNode* GetNextContact(ContactNode* thisNode);
void PrintContactNode(ContactNode* thisNode);

#endif  // CONTACT_NODE_H

