#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int height = 3, width = 5;
	int baseInt, headInt;
	scanf("%d", &baseInt);
	scanf("%d", &headInt);
	
	for(int i = -height; i<=height; i++) {
		for(int j = 0; j < width; j++) {
			if(abs(i) > height / 2) {
				printf("%c",' ');
			} else {
				printf("%d", baseInt);
			}
		}
		for(int j = 0; j < height - abs(i) + 1; j++) {
			printf("%d", headInt);
		}
		printf("\n");
	}
	return 0;
}
