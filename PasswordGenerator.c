#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PASSWORD_LENGTH 8

int main(int argc, char *argv[])
{
    char usage[] = "Usage: PasswordGenerator n, where n is 8 or more characters to generate (default is 8 if no flags specified)";
    if (argc > 2)
    {
        printf("%s\n", usage);
        return EXIT_FAILURE;
    }
    int password_length = DEFAULT_PASSWORD_LENGTH;
    if (argc == 2)
    {
        errno = 0;
        char *end_ptr;
        long conversion = strtol(argv[1], &end_ptr, 10);
        /* strtol only sets errno for overflow conditions, not to indicate parsing failures.
            For that purpose, you have to check the value of the end pointer, but you need to store a pointer to the original string. */
        if (errno != 0)
        {
            printf("Error: Converting command line argument %s to long.\n");
            return EXIT_FAILURE;
        }
        if (end_ptr == argv[1])
        {
            printf("Error: No converson of the command line argument %s was performed.\n");
            return EXIT_FAILURE;
        }
        if (*end_ptr != '\0')
        {
            printf("Error: Not all of the command line argument %s was converted to long.\n");
            return EXIT_FAILURE;
        }
        password_length = conversion;
    }

    return EXIT_SUCCESS;
}