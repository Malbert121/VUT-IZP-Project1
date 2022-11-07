/*
 * IZP 2022: 1-st Project
 * Author: Albert Popov (xpopov10@stud.fit.vutbr.cz)
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LIMIT 101 // implementation limit 100 letters (101-st character for '\0' (End of string))

// | checks if input is empty
// V
bool my_feof(FILE *stream){
    int c;
    bool End_Of_Life = (c = getc(stream)) != EOF ? false : true;
    ungetc(c, stream);
    return End_Of_Life; // :)
}

bool my_num_search(char char_number){ //<-- checks if string element is a number (by ASCII)
    return (char_number <= '9' && char_number >= '0')? true : false;
}

long long my_atoll(char *string){
    long long result = 0;
    for (int i = 0; string[i] != '\0' ; i++)
    {
        if (string[i] >= '0' && string[i] <= '9')
        {
            result *= 10 + (string[i] - 48);
        } else
        {
            return -1;
        }
    }
    return result;
}

char *my_fgets(char* string, int limit, FILE *stream) //my analog of fgets function from standard C library <string.h>
{                                                     //but with little changes (for task implementation purpose)
    int z, i = 0;
    for (; i < limit - 1 && (z = getc(stream)) != EOF && z != '\n'; string[i++] = z >= 'A' && z <= 'Z' ? z + 32 : z)
    {}                                                                    //  ^                   ^
    string[i] = '\0';//getc doesn't know where is end of string character //  | if character is big, then it makes it little
    return i > 0 ? string : NULL; //<-- if input is empty, then function returns NULL
}

char *my_strcat(char *destination, const char *source) //my analog of strcat function from standard C library <string.h>
{
    int i,j = 0;
    for (i = strlen(destination); source[j] != '\0'; destination[i++] = source[j++])
    {}
    destination[i] = '\0';
    return destination;
}

bool check_name(char *name, int limit) // My first check function as part of the assignment
{                                      // Checks if name string is valid
    my_fgets(name, limit, stdin);
    strcat(name, ", "); //<-- final string format is "%name, %number". ", " <-- is a final string part
    return true;
}

bool check_number(char *number, int limit) // Checks if numbers string is valid
{
    my_fgets(number, limit, stdin);
    for (int i = 0; number[i] != '\0'; ++i)
    {
        if (number[i] == '+' && i == 0) {continue;}
        if (!my_num_search(number[i]))
            return false;// <-- returns if one character is non-number (by ASCII)
    }
    return true;
}

bool check_contact(char *name, char *number)// Checks if two previous reminded functions are valid
{
    if (check_name(name, LIMIT) && check_number(number, LIMIT))
    {
        return strlen(my_strcat(name, number)) < LIMIT * 2 ? true : false;
    }   //         ^        // ^ concatenating name and number string into entire contact
    //         | Checks if size of concatenated string within limit
    return false;
}

bool argument_number (char *contact, char *argument)// Checks if number substring a subset of number string
{
    return strstr(contact, argument) ? true : false;
}

bool arguments_err(int argc) // Checks if unnecessary amount of arguments is there
{
    if (argc > 2)
    {
        fprintf(stderr,"ERROR: Too many arguments!!!\n");
        return false;
    }
    return true;
}

// I'm hungry(

bool argument_char(char* contact, char **mobile_keyboard, char *arguments, char *name_input_numbers)
{
    memset(name_input_numbers, 0, strlen(name_input_numbers)-1);
    int i;
    char *position = name_input_numbers;
    bool found = false;
    for (i = 0; contact[i] != '\0'; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            for (int k = 0, z; (z = mobile_keyboard[j][k]) != '\0'; ++k)
            {
                if (contact[i] == '+')
                {
                    name_input_numbers[i] = '0';
                }
                else if (contact[i] == z && z != ' ' && z != '.')
                {
                    name_input_numbers[i] = j + 50;
                }
                else if (contact[i] == ' ' || contact[i] == '.' || (my_num_search(contact[i])) || contact[i] == ',')
                {
                    name_input_numbers[i] = contact[i];
                }
            }
        }
    }
    name_input_numbers[i] = '\0';
    while ((position = strstr(position, arguments)) != NULL)
    {
        found = true;
        position += strlen(arguments);
    }
    return found ? true : false;
}

int do_things(char *m_arr, char *h_arr, int arguments_amount, char **keyboard, char **argv)
{
    bool works = false, found = false;
    if (my_feof(stdin))
    {
        fprintf(stderr, "ERROR: List is empty\n");
        return 5;
    }
    if (arguments_amount == 1)
    {
        while (!my_feof(stdin))
        {
            if (!check_contact(m_arr, h_arr))
            {
                fprintf(stderr, "ERROR: Wrong input data\n");
                return 3;
            }
            printf("%s\n", m_arr);
        }
    }
    else if (my_atoll(argv[1]) == -1)
    {
        fprintf(stderr, "ERROR: Wrong argument data\n");
        return 2;
    }
    else
    {
        while (!my_feof(stdin))
        {
            if ((works = check_contact(m_arr, h_arr)) &&
                ((argument_char(m_arr, keyboard, argv[1], h_arr))|| argument_number(m_arr, h_arr)))
            {
                found = true;
                printf("%s\n", m_arr);
            }
            else if (works)
            {
                continue;
            }
            else
            {
                fprintf(stderr, "ERROR: Wrong input data\n");
                return 6;
            }
        }
        if (!found)
        {
            printf("Not found\n");
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (!arguments_err(argc)) return 1; //<-- No sense to start program with more than 2 arguments
    //{ Variable declarations
    int err_code; //<-- Variable name, talks by herself
    char main_arr[LIMIT * 2]; //<-- Using for store contacts and find names
    char help_arr[LIMIT]; //<-- Using for find phone numbers, and find substrings
    char *mobile_keyboard[9] = {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz", "+"};
    //      ^
    //      |  Declared for implementation purpose
    //}

    // | getting error code (or zero, if everything is ok)
    // V
    err_code = do_things(main_arr, help_arr, argc, mobile_keyboard, argv); //

    return err_code; //if zero it's cool!
}