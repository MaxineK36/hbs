#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/features.h"

#include "../lib/log.c/src/log.h"

/* Setup to allow for handler array */
typedef int (*command_function)(char** sups);


struct command
{
    char* command;
    command_function func;
};

/* Array containing existing commands and their relevant functions */
struct command features[] =
{
    {"create", create},
//    {"enter", enter},
    {"help", help},
    {"quit", quit},
    {"q", quit},
};


/* Number of features */
int num_features = sizeof(features) / sizeof(struct command);

/* Initial path variable (where to put projects) */
char* initial_path = "../";

/* Current project (to be updated) */
char* curr_proj = NULL;

/* Current file (to be updated) */
char* curr_file = NULL;

#define PROJ 1
#define FILE 2

int exec(char* arg, char* sups[])
{
    log_info("in exec");
    log_trace("current project is %s",curr_proj);
    log_trace("current file is %s",curr_file);
    for (int i = 0; i < num_features; i++)
    {
        if (strcmp(features[i].command, arg) == 0)
        {
            log_debug("command was %s",arg);
            return features[i].func(sups);
        }
    }
    return 0;
}

int create(char** sups)
{
    log_info("in create");
    char* args[3];
    switch (pof(sups[0]))
    {
    case 1:
        log_debug("creating a project");
        /* Makes and enters folder for project */
        args[0] = "mkdir";
        args[1] = malloc(1000);
        args[2] = NULL;
        curr_proj = strdup(sups[1]);
        sprintf(args[1],"%s%s",initial_path, curr_proj);
        log_trace("args[1] is %s",args[1]);
        lsh_launch(args);

        /* Create index.html file */
        log_debug("creating its index file");
        char* fargs[2];
        fargs[0] = "file";
        fargs[1] = "index";
        create(fargs);

        return 1;
    case 2:
        log_debug("creating a file");
        /* Makes sure a project is specified */
        if (!curr_proj) {
            log_warn("no project specified");
            printf("No project was specified.\n");
            return 0;
        }

        /* Creates a new file in the specified project */
        args[0] = "touch";
        args[1] = malloc(1000);
        args[2] = NULL;
        sprintf(args[1],"%s%s/%s.html",initial_path, curr_proj, sups[1]);
        log_trace("args[1] is %s",args[1]);
        lsh_launch(args);

        /* Sets current file to the file you just created */
        curr_file = sups[1];        

        return 1;
    default:
        log_trace("sups[0] was %s", sups[0]);
        log_warn("no sups[0] specified");
        return 0;
    }
}

//to add, echo "hello" >> <filename>

int quit(char** sups)
{
    sups++;
    return -2;
}

int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int help(char** sups)
{
    int argisprint;
    if (sups[0] == NULL)
    {
        std_indent("This tool provides a set of commands to visualize a trie.\n"
                   "The function takes in a main command, print, as well as a trie to be printed.\n"
                   "More arguments can be added to specify visualization types and to see subtries.\n"
                   "Usage: print <trie> [option] [prefix]\n"
                   "Visualize tries using either all-nodes, only-words, or only-leaves OPTION.\n"
                   "If no prefix is specified, the entire trie will be visualized.\n"
                   "Example: print trie1 only-words 'ab'\n\n"
                   "Options:\n"
                   "all-nodes\t prints out all nodes of the trie\n"
                   "only-words\t only prints out valid words of the trie\n"
                   "only-leaves\t only prints out the leaves of the trie\n"
                   "n-completions\t prints out n completions of a trie\n"
                   "Note: n completions must also be used with a number:\n"
                   "print <trie> n-completions [prefix] [n]\n"
                   "Type 'help [option]' for more information on each option.\n\n"
                   "Prefix:\n"
                   "Include the prefix in a trie to print out only the subtrie under that prefix.\n");
    }
    else
    {
        argisprint = !strcmp(sups[0],"print") ? 1 : 0;
        if ((!strcmp(sups[0],"all-nodes"))||(argisprint&&(!strcmp(sups[1],"all-nodes"))))
        {
            std_indent("To see all the nodes from a trie, type:\n"
                       "'print <trie>' or 'print <trie> all-nodes' where trie is the key of the trie you're printing\n"
                       "This will show an alphabetical visualization of every node in your trie\n"
                       "For example, the trie 'trie-ex' contains the, they, and we\n"
                       "Typing 'print trie-ex all-nodes' will return:\n"
                       "t\nt-h\nt-h-e\nt-h-e-y\nw\nw-e\n"
                       "Optionally, you can use it with a prefix to view all nodes of a subtrie by typing\n"
                       "'print <trie> all-nodes [prefix]' where [prefix] is replaced by your prefix\n"
                       "For example, typing 'print trie-ex all-nodes th' will return:\n"
                       "t-h\nt-h-e\nt-h-e-y\n");
        }
        else if ((!strcmp(sups[0],"only-leaves"))||(argisprint&&(!strcmp(sups[1],"only-leaves"))))
        {
            std_indent("To see only the leaves from a trie, type:\n"
                       "'print <trie> only-leaves' where trie is the key of the trie you're printing\n"
                       "This will show an alphabetical visualization of every leaf, all the way to the end\n"
                       "For example, the trie 'trie-ex' contains the, they, and we\n"
                       "Typing 'print trie-ex only-leaves' will return:\n"
                       "t-h-e-y\nw-e\n"
                       "Optionally, you can use it with a prefix to view leaves of a subtrie by typing\n"
                       "'print <trie> only-leaves [prefix]' where [prefix] is replaced by your prefix\n"
                       "For example, typing 'print trie-ex only-leaves th' will return:\n"
                       "t-h-e-y\n");
        }
        else if ((!strcmp(sups[0],"only-words"))||(argisprint&&(!strcmp(sups[1],"only-words"))))
        {
            std_indent("To see only the words from a trie, type:\n"
                       "'print <trie> only-words' where trie is the key of the trie you're printing\n"
                       "This will show an alphabetical visualization of every word, even if it is not the end of the leaf\n"
                       "For example, the trie 'trie-ex' contains the, they, and we\n"
                       "Typing 'print trie-ex only-words' will return:\n"
                       "t-h-e\nt-h-e-y\nw-e\n"
                       "Optionally, you can use it with a prefix to view words of a subtrie by typing\n"
                       "'print <trie> only-words [prefix]' where [prefix] is replaced by your prefix\n"
                       "For example, typing 'print trie-ex only-words th' will return:\n"
                       "t-h-e\nt-h-e-y\n");
        }
        else if (!strcmp(sups[0],"n-completions"))
        {
            std_indent("To see n completions of a prefix from a given trie, type:\n"
                       "'n-completions <trie> <n>' where trie is the key of the trie you're printing, and n is the number of completions you want\n"
                       "This will show the first n words in that trie that start with that prefix\n"
                       "For example, the trie 'trie-ex' contains the, they, and we\n"
                       "Typing 'print n-completions trie-ex 2 th' will return:\n"
                       "t-h-e\nt-h-e-y\n"
                       "Typing 'print n-completions trie-ex 1 th' will return:\n"
                       "t-h-e\n");
        }
        else if (!strcmp(sups[0],"quit"))
        {
            std_indent("Typing 'quit' or 'q' allows you to exit the shell\n");
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

void std_indent(char* string)
{
    print_indented_n(string, 2);
}

void print_indented_n(char* string, int indent)
{
    int i = 0;
    int i_n = 0;
    char* new_str = calloc(1,2*strlen(string));
    for (; i_n < indent; i_n++) {
        new_str[i_n] = ' ';
    }
    while (string[i])
    {
        if (string[i] == '\n')
        {
            new_str[i_n++] = '\n';
            for (int j = 0; j < indent; j++)
                new_str[i_n++] = ' ';
        }
        else
        {
            new_str[i_n++] = string[i];
        }
        i++;
    }
    int len = strlen(new_str);
    for (int j = len; j >= len - indent; j--) {
        new_str[j] = (new_str[j] == ' ') ? '\0' : ' ';
    }
}

char* sepconcat(char** strlist, int n, char* sep)
{
    int total_len = 0;
    for (int i = 0; i < n; i++){
       total_len += strlen(strlist[i]); 
    }
    char* total_str = malloc(total_len+n+1);
    total_str = strdup(strlist[0]);
    for (int i = 1; i < n; i++){
        strcat(total_str, sep);
        strcat(total_str, strlist[i]);
    }
    return total_str; 
}

int pof(char* argument){
    if (!strcmp(argument,"project")) {
        return 1;
    } else if (!strcmp(argument,"file")) {
        return 2;
    } else {
        return 0;
    }
}

