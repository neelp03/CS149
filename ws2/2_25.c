/**
 * Description: Exercise 2.25 from Zybooks
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: February 8, 2023
 * Creation date: February 8, 2023
 **/

#include <stdio.h>

/**
 * This function divides two numbers given by user and prints the result
 * Example: If input is 2000 2 then it will divide 2000 by 2 three times
 *          and print the results
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
**/
int main(void) {
	int user_num;
	int x;
	scanf("%d", &user_num);
	scanf("%d", &x);
	printf("%d %d %d", user_num/x, user_num/x/x, user_num/x/x/x);
}

