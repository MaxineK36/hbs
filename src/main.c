#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include "../include/features.h"


int parse(char* input)
{
    char* separator = " ";
    char* parsed = strtok(input, separator);
    char* arg = parsed;
    char** sups = calloc(8,sizeof(char*));

    int index = 0;
    parsed = strtok(NULL, separator);
    while(parsed)
    {
        sups[index] = parsed;
        index++;
        parsed = strtok(NULL, separator);
    }

    int final = exec(arg, sups);
    if (final == 0)
        printf("Invalid input; please try again.\n"
               "For a list of commands, type help\n");
    else if (final == -1)
        printf("Program error - please try again\n");
    return final;
}


int main()
{
    std_indent("\nWelcome to my webpage developer tool!\n"
              "To exit, type 'quit'\n\n");
    int run = 1;
    while (run != -2)
    {
        char* input = readline("> ");
        run = parse(input);
    }
    printf("Exited.\n");
    return 0;
}

