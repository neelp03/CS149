/**
 * Description: Exercise 6.34 Multiples of ten in an array from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 7, 2023
 * Creation date: March 7, 2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 19
 
 /**
 * This function tests if there are all multiples of 10
 * Assumption: None
 * Input parameters: the list of values and its size 
 * Returns: boolean
 **/
 bool IsArrayMult10(int list[], int size){
 
 	bool all = true;
 	
 	for(int i = 0; i < size; i++){
 		if(list[i]%10 != 0){
 			return !all;
 		}
 	}
 	
 	return all;
 
 }
 
 
 /**
 * This function tests if there are no multiples of 10
 * Assumption: None
 * Input parameters: the list of values and its size
 * Returns: boolean
 **/
 bool IsArrayNoMult10(int list[], int size){
 
 bool none = true;
 	
 	for(int i = 0; i < size; i++){
 		if(list[i]%10 == 0){
 			return !none;
 		}
 	}
 	
 	return none;
 
 }
 
 
 /**
 * This function prints if an array has multiples
 * of 10 in it
 * Assumption: list will always contain less than 20 integers
 * Input parameters: none
 * Returns: an integer
 **/
 int main(void){
 	
 	int size;
 	int temp;
 	int list[MAX_SIZE];
 	
 	scanf("%d", &size);
 	
 	if(size > MAX_SIZE){
 		fprintf(stderr, "Error: list too long\n");
 		exit(1);
 	}
 	
 	for(int i = 0; i < size; i++){
 		scanf("%d", &temp);
 		list[i] = temp;

 	}
 	
 	if(!IsArrayMult10(list, size) && !IsArrayNoMult10(list, size)){
 		printf("mixed values\n");
 	}
 	else if(IsArrayMult10(list, size)){
 		printf("all multiples of 10\n");
 	} else {
 		printf("no multiples of 10\n");
 	}
 
 	return 0;
 }
