#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_PASSWORD_LENGTH 8
#define DEFAULT_NUMBER_OF_PASSWORDS_TO_GENERATE 1
#define MINIMUM_PASSWORD_LENGTH 8
#define MAXIMUM_PASSWORD_LENGTH 16

long convert_command_line_argument(const char *argument, unsigned int command_line_index);
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
    long number_of_passwords_to_generate = DEFAULT_NUMBER_OF_PASSWORDS_TO_GENERATE;
    if (argc == 2)
    {        
        password_length = convert_command_line_argument(argv[1], 1);
        if (!password_length)
        {
            return EXIT_FAILURE;
        }
    }
    else if (argc == 3)
    {
        number_of_passwords_to_generate = convert_command_line_argument(argv[2], 2);
        if (!number_of_passwords_to_generate)
        {
            return EXIT_FAILURE;
        }
    }

    char *password = (char *) malloc(MAXIMUM_PASSWORD_LENGTH + 1); // Allocate the memory for our password, + 1 for null terminating character
    static const char *const character_sets[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", // https://stackoverflow.com/questions/1200690/c-how-to-correctly-declare-an-array-of-strings
                                                    "abcdefghijklmnopqrstuvwxyz",
                                                    "0123456789",
                                                    "!£$%^&*()`;:@'~#<>?,."};
    srand(time(0)); // Seed the rand with the current time
    unsigned int number_of_rows = sizeof(character_sets)/sizeof(character_sets[0]); // how many character sets?
    
    /* Set the first four characters to the corresponding character set to satisfy, in a basic way,
    that a password must have upper case, lower case, numeric and a special characters */
    int position;
    // Start measuring time
    /*time_t begin, end;
    time(&begin);*/
    for (long password_count = 0; password_count < number_of_passwords_to_generate; ++password_count)
    {
        for (position = 0; position < number_of_rows; ++position)
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
    }
    // Stop measuring time and calculate the elapsed time
    // This will measure the wall clock on both Winodws & Linux, but only in full seconds.
    /*time(&end);
    time_t elapsed = end - begin;
    fprintf(stdout, "Generated %ld %i character long passwords in %ld seconds.\n", number_of_passwords_to_generate, password_length, elapsed);*/
    free(password); // Make sure we free what the dynamically allocated

    return EXIT_SUCCESS;
}

long convert_command_line_argument(const char *argument, unsigned int command_line_index)
{
    errno = 0;
    char *end_ptr;
    long conversion = strtol(argument, &end_ptr, 10); //strtol as strtoi has no error checking, as any invalid input is treated as 0
    /* strtol only sets errno for overflow conditions, not to indicate parsing failures.
        For that purpose, you have to check the value of the end pointer, but you need to store a pointer to the original string. */
    if (errno != 0)
    {
        fprintf(stderr, "Error: Converting command line argument %s to long.\n");
        return 0;
    }
    if (end_ptr == argument) // Point to the same starting address as the address of the input then no conversion was done
    {
        fprintf(stderr, "Error: No converson of the command line argument %s was performed.\n");
        return 0;
    }
    if (*end_ptr != '\0') // If we're not at the null termination then there's some of the input that wasn't converted
    {
        fprintf(stderr, "Error: Not all of the command line argument %s was converted to long.\n");
        return 0;
    }
    switch (command_line_index)
    {
        case 1:
            if (conversion < MINIMUM_PASSWORD_LENGTH || conversion > MAXIMUM_PASSWORD_LENGTH)
            {
                fprintf(stderr, "Error: Command line argument 1 must be between %i and %i but it is %ld\n", MINIMUM_PASSWORD_LENGTH, MAXIMUM_PASSWORD_LENGTH, conversion);
                return 0;
            }
            break;
        case 2:
            if (conversion < LONG_MIN || conversion > LONG_MAX)
            {
                fprintf(stderr, "Error: Command line argument 2 must be between %lf and %ld but it is %ld\n", LONG_MIN, LONG_MAX, conversion);
                return 0;
            }
            break;
        default:
            fprintf(stderr, "Error: Unknown command_line_index %u\n", command_line_index);
            return 0;
    }    
    return conversion;
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
    return start_of_range + rand() / (RAND_MAX / (end_of_range - start_of_range + 1) + 1);
}