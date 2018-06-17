#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include "../include/features.h"

#include "../lib/log.c/src/log.h"


int parse(char* input)
{
    log_info("in parse");
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
    log_trace("got args");

    int final = exec(arg, sups);
    log_trace("exec returned %d",final);
    if (final == 0)
        printf("Invalid input; please try again.\n"
               "For a list of commands, type help\n");
    else if (final == -1)
        printf("Program error - please try again\n");
    return final;
}


int main(int argc, char* argv[])
{
    std_indent("\nWelcome to my webpage developer tool!\n"
              "To exit, type 'quit'\n\n");

    int verbosity = 0;
    
    int opt;
    while ((opt = getopt(argc, argv, "vh")) != -1)
        switch (opt)
        {
        case 'v':
            verbosity++;
            break;
        }
    switch (verbosity) {
      case 0:
        log_set_level(LOG_FATAL + 1); break;
//      case 1:
//        log_set_level(LOG_INFO); break;
//      case 1:
//        log_set_level(LOG_DEBUG); break;
      case 1:
        log_set_level(LOG_TRACE); break;
    } 
    log_trace("Verbosity is %d",verbosity);

    int run = 1;
    while (run != -2)
    {
        log_trace("about to look for input");
        char* input = readline("> ");
        run = parse(input);
        log_trace("parse returned %d",run);
    }
    printf("Exited.\n");
    return 0;
}

