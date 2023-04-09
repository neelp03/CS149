/**
 * Description: Exercise 7.6 Online shopping cart - functions from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: April 6, 2023
 * Creation date: April 9, 2023
 **/
 
#include <stdio.h>
#include <string.h>
#include "ItemToPurchase.c"

void MakeItemBlank(struct ItemToPurchase *item);
void PrintItemCost(struct ItemToPurchase item);

/**
 * This function calculates the total cost of a purchase
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
 int main(){
 
 	 struct ItemToPurchase item1;
 	struct ItemToPurchase item2;
 	char tempName[50];
 	char *b = tempName;
 	size_t bufsize = 50;
 	int tempPrice;
 	int tempQuantity;
 	char c;
 	
 	
 	item1.print = PrintItemCost;

 	printf("Item 1 \nEnter the item name:\n");
 	getline(&b, &bufsize, stdin);
 	
 	if(tempName[strlen(tempName)-1] == '\n'){
    	tempName[strlen(tempName)-1] = '\0';
    	}
    
 	strcpy(item1.itemName, tempName);
 	printf("Enter the item price:\n");
 	scanf("%d", &tempPrice);
 	item1.itemPrice = tempPrice;
 	printf("Enter the item quantity:\n");
 	scanf("%d", &tempQuantity);
 	item1.itemQuantity = tempQuantity;

 	printf("\n");
	c = getchar();
 	while(c!='\n' && c!= EOF){
 		c = getchar();
 	}
 
 	item2.print = PrintItemCost;
 	printf("Item 2 \nEnter the item name:\n");
 	getline(&b, &bufsize, stdin);
 	
 	if(tempName[strlen(tempName)-1] == '\n'){
    	tempName[strlen(tempName)-1] = '\0';
    	}
    
 	strcpy(item2.itemName, tempName);
 	printf("Enter the item price:\n");
 	scanf("%d", &tempPrice);
 	item2.itemPrice = tempPrice;
 	printf("Enter the item quantity:\n");
 	scanf("%d", &tempQuantity);
 	item2.itemQuantity = tempQuantity;
 	
 	printf("\nTOTAL COST\n");
 	item1.print(item1);
 	item2.print(item2);
 	printf("\n");
 	printf("Total: $%d\n", (item1.itemPrice*item1.itemQuantity)+(item2.itemPrice*item2.itemQuantity));
 	
 	return 0;
 	
 	
 }
