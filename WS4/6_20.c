/**
 * Description: Exercise 6.20 - "Track laps to miles" from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: February 23, 2023
 * Creation date: February 23, 2023
 **/
 
#include <stdio.h>

/**
 * This function calculates the number of miles
 * from given amount of laps
 * Assumption: none
 * Input parameters: double
 * Returns: the calculated amount
 **/
double LapsToMiles(double laps){
	double miles;
	miles = laps*0.25;
	return miles;
}


/**
 * This function converts track laps to miles
 * Assumption: none
 * Input parameters: none
 * Returns: an integer
 **/
int main() {
    
    double laps;
    double miles;
    
    scanf("%lf", &laps);
    
    miles = LapsToMiles(laps);
    printf("%0.2f\n", miles);
    return 0;
}




