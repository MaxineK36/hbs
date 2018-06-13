
/* When given the function and its arguments, excecutes the relevant function
 * Returns -2 if the input is "quit"
 * Returns 1 if the input is not "quit" but is valid and successfully excecuted
 * Returns 0 if something goes wrong due to bad input
 * Returns -1 if something goes wrong for internal reasons */
int exec(char* arg, char* sups[]);

/* Takes an input of supplementary arguments
 * Interprets the first to indicate creating either a file or a folder
 * Uses the second as the name of the file or folder
 * Returns 1, 0, or -1 as described above */
int create(char** sups);

/* Returns -2, does nothing else*/
int quit(char** sups);

/* Prints help text, returns 1 always */
int help(char** sups);

/* Helper function to print with a standardized indentation
 * Takes an input of a string and prints with 2 spaces as an indent
 */
void std_indent(char* string);

/* Helper function to print with a given indentation
 * Takes a string and a number n, prints the string with each line indented n spaces
 */
void print_indented_n(char* string, int indent);

/* Function to launch a child process and execute a function
 * Taken from:
 * https://github.com/brenns10/lsh/blob/407938170e8b40d231781576e05282a41634848c/src/main.c
 */
int lsh_launch(char **args);


char* concat(char **strlist, int n);
/*
const char* inputs = {
  "help",
  "quit",
  "eviz",
  "lviz",
  "wviz",
  "sviz",
};

const int functions = {
  (*help)(char**),
  (*quit)(char**),
  (*eviz)(char**),
  (*lviz)(char**),
  (*wviz)(char**),
  (*sviz)(char**),
};

const int num_features = 6;
*/

