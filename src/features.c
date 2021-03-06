#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/features.h"

#include "../lib/log.c/src/log.h"

#define PROJ 1
#define FILE 2

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
    {"c", create},
    {"enter", enter},
    {"e", enter},
    {"exit", leave},
    {"leave", leave},
    {"l", leave},
    {"add", add},
    {"a", add},
    {"print", print},
    {"p", print},
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
    if (!sups[0] || !sups[1]) {
        log_warn("insufficient arguments");
        log_debug("sups[0] is %s",sups[0]);
        log_debug("sups[1] is %s",sups[1]);
        return 0;
    }
    switch (pof(sups[0]))
    {
    case 1:
        log_debug("creating a project");
        return create_proj(sups[1]);
    case 2:
        log_debug("creating a file");
        return create_file(sups[1]);
    default:
        log_trace("sups[0] was %s", sups[0]);
        log_warn("improper sups[0] specified");
        return 0;
    }
}

int create_proj(char* proj_name)
{
    log_info("in proj_create");

    /* Makes folder for project */
    char* args[3];
    args[0] = "mkdir";
    args[1] = malloc(1000);
    args[2] = NULL;
    sprintf(args[1],"%s%s",initial_path, proj_name);
    log_trace("args[1] is %s",args[1]);
    lsh_launch(args);

    /* Enters project */ 
    curr_proj = strdup(proj_name);
    log_trace("current project is %s",curr_proj);

    /* Create index.html file */
    create_file("index");
    return 1;
}

int create_file(char* file_name)
{
    log_info("in file_create");

    /* Makes sure a project is specified */
    if (!curr_proj) {
        log_warn("no project specified");
        printf("No project was specified.\n");
        return 0;
    }

    /* Creates a new file in the specified project */
    char* args[3];
    args[0] = "touch";
    args[1] = malloc(1000);
    args[2] = NULL;
    sprintf(args[1],"%s%s/%s.html",initial_path, curr_proj, file_name);
    log_trace("args[1] is %s",args[1]);
    lsh_launch(args);

    /* Sets current file to the file you just created */
    curr_file = strdup(file_name);        
    log_trace("current file is %s", curr_file);
    return 1;
}

int enter(char** sups)
{
    log_info("in enter");
    if (!sups[0] || !sups[1]) {
        log_warn("insufficient arguments");
        log_debug("sups[0] is %s",sups[0]);
        log_debug("sups[1] is %s",sups[1]);
        return 0;
    }
    switch (pof(sups[0]))
    {
    case PROJ:
        log_debug("entering a project");
        curr_proj = strdup(sups[1]);
        curr_file = "index";
        log_trace("current project is %s",curr_proj);
        log_trace("current file is %s", curr_file);
        return 1;
    case FILE:
        log_debug("entering a file");
        curr_file = strdup(sups[1]);
        log_trace("current file is %s", curr_file);
        return 1;
    default:
        log_trace("sups[0] was %s", sups[0]);
        log_warn("improper sups[0] specified");
        return 0;
    }
}

int leave(char** sups)
{
    log_info("in exit");

    //sets to project if no argument specified, runs pof otherwise
    int temp_pof = sups[0] ? pof(sups[0]) : PROJ;
    
    switch (temp_pof)
    {
    case PROJ:
        log_debug("exiting a project");
        curr_proj = NULL;
        curr_file = NULL;
        log_trace("current project is %s",curr_proj);
        log_trace("current file is %s", curr_file);
        return 1;
    case FILE:
        log_debug("exiting a file");
        curr_file = NULL;
        log_trace("current file is %s", curr_file);
        return 1;
    default:
        log_trace("sups[0] was %s", sups[0]);
        log_warn("improper sups[0] specified");
        return 0;
    }
}

/* Takes an input of supplementary arguments
 * Interprets the first as the type of object to add
 * Interprets the second as the id of the object
 * Interprets the (optional) third to be 'to' 'after' or 'before'
 * Interprets the (optional) fourth as what to add (to/after/before)
 * Returns 1, 0, or -1 as described above */
int add(char** sups)
{
    log_info("in add");
    if (!sups[0] || !sups[1]) {
        log_warn("insufficient arguments");
        log_debug("sups[0] is %s",sups[0]);
        log_debug("sups[1] is %s",sups[1]);
        return 0;
    }

    char* type = sups[0]; //type of object
    char* id = sups[1]; //id for object
    char* rel = sups[2]; //relation
    char* loc = sups[3]; //location

    log_debug("type is %s",type);
    log_debug("id is %s",id);

    char* arg[5];
    arg[0] = "sed";
    arg[1] = "-i";
    arg[2] = calloc(1,200);
    char* to_add = calloc(1,200);
    arg[3] = malloc(strlen(curr_file) + strlen(curr_proj) + 20);
    arg[4] = NULL;

    sprintf(to_add,"<%s\\ id=\"%s\"></%s>",type,id,type); //set up text to add
    sprintf(arg[3],"%s%s/%s.html", initial_path, curr_proj, curr_file); //set up which file to edit

    /* If a relation was specified */
    if (rel){
        log_debug("relation specified");
        log_trace("relation is %s",rel);
        /* Makes sure that a location was specified */
        if (!loc) {
            log_warn("there was a relation but no location");
            return 0;
        }
        log_trace("location is %s",loc);

        /* Determine *which* relation was specified, act accordinbly */
        if (!strcmp(rel,"to")) {
            sprintf(to_add,"<%s\\ id=\"%s\"><\\/%s>",type,id,type); //set up text to add
            sprintf(arg[2],"s/id=\"%s\">/\\id=\"%s\">%s/",loc,loc,to_add);
        } else if (!strcmp(rel,"before")) {
            sprintf(arg[2],"/id=\"%s\"/i\\%s",loc,to_add);
        } else if (!strcmp(rel,"after")) {
            sprintf(arg[2],"/id=\"%s\"/a\\%s",loc,to_add);
        } else {
            return 0;
        }
    } else {
        log_debug("no relation specified");
        sprintf(arg[2],"/<\\/html>/i\\%s",to_add);
    }

    lsh_launch(arg);
    return 1;
}

int print(char** sups)
{
    if (!sups[0]){
        printf("File name not specified.\n");
        return 0;
    }
    char* filename = sups[0];
    char* full_file_path = calloc(1,100);

    sprintf(full_file_path,"%s%s/%s.html", initial_path, curr_proj, filename);
    struct stat buffer;
    int exist = stat(full_file_path,&buffer);
    if (exist != 0) {
        printf("This file does not exist in the given project.\n");
        return 0;
    }

    char* args[3] = {"cat",full_file_path,NULL};
    lsh_launch(args);
    //http://www.zentut.com/c-tutorial/c-file-exists/

    return 1;
}

int quit(char** sups)
{
    sups++;
    return -2;
}

int lsh_launch(char **args)
{
    int i = 0;
    while(args[i]){
        log_trace("%s ",args[i]);
        i++;
    }

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
    if ((!strcmp(argument,"project"))||(!strcmp(argument,"p"))||(!strcmp(argument,"folder"))) {
        return PROJ;
    } else if ((!strcmp(argument,"file"))||(!strcmp(argument,"f"))) {
        return FILE;
    } else {
        return 0;
    }
}

