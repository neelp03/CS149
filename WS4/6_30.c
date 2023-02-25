/**
 * Description: Exercise 6.30 - "Max and min" from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 25, 2023
 * Creation date: February 25, 2023
 **/
 
#include <stdio.h>

/**
 * Description: This function takes four integer parameters and returns the maximum value of the four.
 * Assumption: None
 * Input parameters: 
 *     num1: an integer
 *     num2: an integer
 *     num3: an integer
 *     num4: an integer
 * Returns: an integer representing the maximum value of the four input integers.
**/
int MaxNumber(int num1, int num2, int num3, int num4) {
    // Assume the first number is the maximum.
    int max = num1;

    // Check if the second number is greater than the current maximum.
    if (num2 > max) {
        max = num2;
    }

    // Check if the third number is greater than the current maximum.
    if (num3 > max) {
        max = num3;
    }

    // Check if the fourth number is greater than the current maximum.
    if (num4 > max) {
        max = num4;
    }

    // Return the maximum value.
    return max;
}

/**
 * Description: This function takes four integer parameters and returns the minimum value of the four.
 * Assumption: None
 * Input parameters: 
 *     num1: an integer
 *     num2: an integer
 *     num3: an integer
 *     num4: an integer
 * Returns: an integer representing the minimum value of the four input integers.
**/
int MinNumber(int num1, int num2, int num3, int num4) {
    // Assume the first number is the minimum.
    int min = num1;

    // Check if the second number is less than the current minimum.
    if (num2 < min) {
        min = num2;
    }

    // Check if the third number is less than the current minimum.
    if (num3 < min) {
        min = num3;
    }

    // Check if the fourth number is less than the current minimum.
    if (num4 < min) {
        min = num4;
    }

    // Return the minimum value.
    return min;
}

int main() {
    int num1, num2, num3, num4;
    int max, min;
    
    // gets four integer user inputs
    scanf("%d %d %d %d", &num1, &num2, &num3, &num4);
    
    // calls max and min function
    max = MaxNumber(num1, num2, num3, num4);
    min = MinNumber(num1, num2, num3, num4);

    printf("Maximum is %d\n", max);
    printf("Minimum is %d\n", min);

    return 0;
}


