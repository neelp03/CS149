/**
 * Description: Exercise 5.28 - Finding palindrome from Zybooks
 * Author names: Megan Ju, Neel Patel
 * Author emails: megan.ju@sjsu.edu, neel.patel@sjsu.edu
 * Last modified date: March 11, 2023
 * Creation date: March 11, 2023
 **/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 50

/**
 * Removes spaces from a string and converts all characters to lowercase.
 * Assumption: The `stripped` array is large enough to hold the stripped string.
 * Input parameters: `input` - the string to strip spaces from.
 *                   `stripped` - the array to store the stripped string in.
 * Returns: None.
 */
void strip_and_lower(char input[], char stripped[]) {
    int i, j, len;
    len = strlen(input);
    j = 0;
    for (i = 0; i < len; i++) {
        if (!isspace(input[i])) {
            stripped[j] = tolower(input[i]);
            j++;
        }
    }
    stripped[j] = '\0';
}

/**
 * Checks if a given string is a palindrome.
 * Assumption: None.
 * Input parameters: `str` - the string to check.
 * Returns: 1 if the string is a palindrome, 0 otherwise.
 */
int is_palindrome(char str[]) {
    int i, j, len;
    len = strlen(str);
    for (i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    char input[MAX_LENGTH + 1];
    char stripped[MAX_LENGTH + 1];
    int is_pal;

    fgets(input, MAX_LENGTH + 1, stdin);

    // Strip spaces and convert to lowercase
    strip_and_lower(input, stripped);

    // Check if stripped string is a palindrome
    is_pal = is_palindrome(stripped);

    // Output result
    if (is_pal) {
        printf("palindrome: %s\n", input);
    } else {
        printf("not a palindrome: %s\n", input);
    }

    return 0;
}


