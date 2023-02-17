/**
 * Description: Exercise 4.16 from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 17, 2023
 * Creation date: February 17, 2023
 **/
 
#include <stdio.h>

/**
 * This function prints out the max and average of given numbers
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/

int main() {
    int n, count = 0, sum = 0, max = 0;
    float avg;
    
    while (1) {
        scanf("%d", &n);
        if (n < 0) break;
        count++;
        sum += n;
        if (n > max) max = n;
    }
    
    avg = (float)sum / count;
    
    printf("%d %0.2lf\n", max, avg);
    
    return 0;
}


