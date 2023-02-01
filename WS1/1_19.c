/**
 * Description: This module calculates the square and cubed result of a user's number. This module also calculates the sum and product of two user given numbers.
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 1, 2023
 * Creation date: January 31, 2023
 **/

#include <stdio.h>

int main(void){
	int userNum;
	int userNum2;
	
	printf("Enter integer:\n");
	scanf("%d", &userNum);
	
	printf("You entered: ");
	printf("%d", userNum);
	printf("\n");
	printf("%d", userNum);
	printf(" squared is ");
	printf("%d", userNum*userNum);
	printf("\nAnd ");
	printf("%d", userNum);
	printf(" cubed is ");
	printf("%d", userNum*userNum*userNum);
	printf("!!\n");
	
	printf("Enter another integer:\n");
	scanf("%d", &userNum2);
	
	printf("%d", userNum);
	printf(" + ");
	printf("%d", userNum2);
	printf(" is ");
	printf("%d", userNum+userNum2);
	printf("\n");
	printf("%d", userNum);
	printf(" * ");
	printf("%d", userNum2);
	printf(" is ");
	printf("%d", userNum*userNum2);
	printf("\n");
	
	return 0;

}
