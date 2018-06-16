
/* When given the function and its arguments, excecutes the relevant function
 * Returns -2 if the input is "quit"
 * Returns 1 if the input is not "quit" but is valid and successfully excecuted
 * Returns 0 if something goes wrong due to bad input
 * Returns -1 if something goes wrong for internal reasons */
int exec(char* arg, char* sups[]);

/* Takes an input of supplementary arguments
 * Interprets the first to indicate creating either a file or a folder
 * Uses the second as the name of the file or folder
 * Creates a file/folder accordingly
 * Returns 1, 0, or -1 as described above */
int create(char** sups);

/* Creates and enters a project with a given name
 * Returns 1 on success 
 */
int create_proj(char* proj_name);

/* Creates and enters a file with a given name
 * Returns 1 on success 
 */
int create_file(char* file_name);

/* Takes an input of supplementary arguments
 * Interprets the first to indicate entering either a file or a folder
 * Uses the second as the name of the file or folder
 * Enters a file/folder accordingl (sets curr_proj or curr_file)
 * If entering a folder, enters file "index.hml" automatically
 * Returns 1, 0, or -1 as described above */
int enter(char** sups);

/* Takes an input of supplementary arguments
 * Interprets the first to indicate entering either a file or a folder
 * Leaves current file/folder accordingly
 * If leaves a folder, exit file as well (sets curr_proj and curr_file to NULL) 
 * Returns 1, 0, or -1 as described above */
int leave(char** sups);

/* Takes an input of supplementary arguments
 * Interprets the first as the type of object to add
 * Interprets the second as the id of the object
 * Interprets the (optional) third to be 'to' 'after' or 'before'
 * Interprets the (optional) fourth as what to add to(/after/before)
 * Returns 1, 0, or -1 as described above */
int add(char** sups);

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

/* Concatenates a list of strings with a given separator
 * Returns the final string
 */
char* sepconcat(char **strlist, int n, char* sep);

/* Determines if the user is referring to a project or a file
 * Returns 1 if project, 2 if file, 0 if argument was NULL)
 */
int pof(char* argument);

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

