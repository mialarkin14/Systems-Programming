#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(char *s1, char *s2)
{
    // Allocating memory for concatnate of two strings
    int a = strlen(s1);     // Getting the length of string 1 and saving it as integer a
    int b = strlen(s2);     // Getting the length of string 2 and saving it as integer b
    char *full_str = (char*)malloc(a + b + 1);      // Allocating memory for the strings concatnated 
    strcpy(full_str, s1);                           // Setting full_str (array) to string 1
    strcat(full_str, s2);                           // Adding string 2 to full_str
    return full_str;                                // Returning full string
}

int main(int argc, char *argv[])
{
    char    *s;

    s = my_strcat("", argv[0]);

    for (int i = 1; i < argc; i ++) {
        char *z = s;
        s = my_strcat(s, argv[i]);
        free(z);
    }
    printf("%s\n", s);
    free(s);
    return 0;
}