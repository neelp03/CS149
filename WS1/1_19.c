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
