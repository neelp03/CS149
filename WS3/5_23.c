/**
 * Description: Exercise 5.23 - Two smallest number from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 17, 2023
 * Creation date: February 17, 2023
 **/
 
#include <stdio.h>

/**
 * This function prints out the two smallest number from a list of numbers
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/

int main(void) {
    int n, i, x, min1, min2;
    
    scanf("%d", &n);
    scanf("%d", &min1);
    scanf("%d", &min2);
    
    if (min2 < min1) {
        int temp = min1;
        min1 = min2;
        min2 = temp;
    }
    
    for (i = 2; i < n; i++) {
        scanf("%d", &x);
        if (x < min1) {
            min2 = min1;
            min1 = x;
        } else if (x < min2) {
            min2 = x;
        }
    }
    
    printf("%d and %d\n", min1, min2);
    
    return 0;
}

