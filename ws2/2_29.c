/**
 * Description: Exercise 2.29 from Zybooks
 * Author names: Neel Patel, Megan Ju
 * Author emails: neel.patel@sjsu.edu; megan.ju@sjsu.edu
 * Last modified date: February 8, 2023
 * Creation date: February 8, 2023
 **/

#include <stdio.h>
#include <math.h>

/**
 * Given three floating-point numbers x, y, and z
 * the function prints x to the power of z, x to the power of (y to the power of 2),
 * the absolute value of y, and the square root of (xy to the power of z).
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
**/
int main(void) {
	double x;
	double y;
	double z;
	scanf("%lf", &x);
	scanf("%lf", &y);
	scanf("%lf", &z);
	printf("%0.2lf ", pow(x, z));
	printf("%0.2lf ", pow(x, pow(y, 2)));
	printf("%0.2lf ", fabs(y));
	printf("%0.2lf", sqrt(pow(x * y, z)));
	return 0;	
}

