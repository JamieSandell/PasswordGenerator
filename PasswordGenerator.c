#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_PASSWORD_LENGTH 8

char get_random_char(const char *character_set);
int get_random_number(int start_of_range, int end_of_range);

int main(int argc, char *argv[])
{
    if (argc > 2) // Too many arguments passed in
    {
        printf("%s\n", "Usage: PasswordGenerator n, where n is 8 or more characters to generate (default is 8 if no flags specified)");
        return EXIT_FAILURE;
    }
    int password_length = DEFAULT_PASSWORD_LENGTH;
    if (argc == 2)
    {
        errno = 0;
        char *end_ptr;
        long conversion = strtol(argv[1], &end_ptr, 10); //strtol as strtoi has no error checking, as any invalid input is treated as 0
        /* strtol only sets errno for overflow conditions, not to indicate parsing failures.
            For that purpose, you have to check the value of the end pointer, but you need to store a pointer to the original string. */
        if (errno != 0)
        {
            fprintf(stderr, "Error: Converting command line argument %s to long.\n");
            return EXIT_FAILURE;
        }
        if (end_ptr == argv[1]) // Point to the same starting address as the address of the input then no conversion was done
        {
            fprintf(stderr, "Error: No converson of the command line argument %s was performed.\n");
            return EXIT_FAILURE;
        }
        if (*end_ptr != '\0') // If we're not at the null termination then there's some of the input that wasn't converted
        {
            fprintf(stderr, "Error: Not all of the command line argument %s was converted to long.\n");
            return EXIT_FAILURE;
        }        
        if (conversion < DEFAULT_PASSWORD_LENGTH || conversion > INT_MAX)
        {
            fprintf(stderr, "Error: Command line argument must be between %i and %i but it is %ld\n", DEFAULT_PASSWORD_LENGTH, INT_MAX, conversion);
            return EXIT_FAILURE;
        }
        password_length = conversion;
    }

    char *password = (char *) malloc(password_length + 1); // Allocate the memory for our password, + 1 for null terminating character
    static const char *const character_sets[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", // https://stackoverflow.com/questions/1200690/c-how-to-correctly-declare-an-array-of-strings
                                                    "abcdefghijklmnopqrstuvwxyz",
                                                    "0123456789",
                                                    "!£$%^&*()`;:@'~#<>?,."};
    srand(time(0)); // Seed the rand with the current time
    unsigned int number_of_rows = sizeof(character_sets)/sizeof(character_sets[0]); // how many character sets?
    
    /* Set the first four characters to the corresponding character set to satisfy, in a basic way,
    that a password must have upper case, lower case, numeric and a special characters */
    int position = 0;
    for (; position < number_of_rows; ++position)
    {
        *(password + position) = get_random_char(character_sets[position]);
    }
    // set the rest of the password
    int random_character_set;
    for (; position < password_length; ++position)
    {
        random_character_set = get_random_number(0, number_of_rows - 1);
        *(password + position) = get_random_char(character_sets[random_character_set]);
    }
    *(password + position) = '\0';
    fprintf(stdout, "%s\n", password);
    free(password); // Make sure we free what the dynamically allocated

    return EXIT_SUCCESS;
}

// Gets a random character from the passed in pointer to a character set. The character set must be null terminated.
char get_random_char(const char *character_set)
{    
    size_t character_set_length = strlen(character_set);
    int start_of_range = 0;
    int end_of_range = character_set_length - 1;
    int random_number = get_random_number(start_of_range, end_of_range);
    return *(character_set + random_number);
}

// Gets a random number from start_of_range (inclusive) to end_of_range (inclusive).
int get_random_number(int start_of_range, int end_of_range)
{
    return (rand() % (end_of_range - start_of_range + 1)) + start_of_range;
}