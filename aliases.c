/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"


// Check if a given line is an alias
bool is_alias (const char * line)
{
    regex_t regex;
    int reti = 0;

    check(line, "Line pointer is NULL.");
    // Compile alias regex
    reti = regcomp(&regex, "^alias.+=(.+);?$", REG_EXTENDED | REG_NEWLINE);
    check(reti == 0, "Couldn't compile alias regex.");
    // Look for regex in line
    reti = regexec(&regex, line, 0, NULL, 0);
    // Free the regex object
    regfree(&regex);
    // If the regex matches, return true
    if (reti == 0) return true;

error: // Fallthrough
    regfree(&regex);
    return false;
}


bool is_function (const char * line)
{
    regex_t regex;
    int reti = 0;

    check(line, "Line pointer is NULL.");
    // Compile function regex
    reti = regcomp(&regex, "^.+[ \\t]*\\(.*\\)[ \\t]*{", REG_ENHANCED | REG_EXTENDED);
    check(reti == 0, "Couldn't compile function regex.");
    // Look for regex in line
    reti = regexec(&regex, line, 0, NULL, 0);
    regfree(&regex);
    // If the regex matches, return true
    if (reti == 0) return true;

error: // Fallthrough
    regfree(&regex);
    return false;
}


// Print a single alias line
void print_alias_line (const char * line)
{
    // Skip over the initial "alias" part
    int i = 6;

    // Print the part that comes
    // before the `=`
    while (line[i] != '=') {
        printf("%c", line[i]);
        i++;
    }

    // Skip over the `=`
    i++;

    // Print an arrow, then the
    // rest of the alias
    printf(" -> ");
    while (i < strlen(line) - 1) {
        printf("%c", line[i]);
        i++;
    }

    printf("\n");
}


// Print a single 'alias' function line
void print_function_line (const char * line)
{
    int i = 0;

    // Print the name of the function
    while (line[i] != ' ' && line[i] != '\t' && line[i] != '(') {
        printf("%c", line[i]);
        i++;
    }

    // Print an arrow, then indicate that it's a function
    printf(" -> <function () {}>\n");
}


// Print aliases in a given filename
int print_aliases (const char * filename, bool print_functions)
{
    FILE * fp = NULL;
    char * line = NULL;
    size_t len = 0;
    // Open file to parse aliases
    fp = fopen(filename, "r");
    // Check if file exists
    check(fp != NULL, "File `%s` doesn't exist.", filename);

    // Print header message
    printf("\e[33mAliases in file `%s`:\e[0m\n", filename);
    printf("\e[33m------------------------------------------\e[0m\n");

    // Read through file
    while (!feof(fp)) {
        // Get a line
        getline(&line, &len, fp);

        // If it's an alias, print it
        if (is_alias(line))
            print_alias_line(line);

        if (print_functions && is_function(line))
            print_function_line(line);

    }

    free(line);

    // Close the file
    fclose(fp);

    return 0;

error:
    return 1;
}


// Print a help screen with a complete list of valid arguments
void print_help_screen ()
{
    printf("\e[33mUsage: aliases [options] [files]\e[0m\n");
    printf("\e[33m\t-f : include function definitions\e[0m\n");
    printf("\e[33m\t-v : print version of aliases\e[0m\n");
    printf("\e[33m\t-h : print this help screen\e[0m\n");
}


int main (int argc, char *argv[])
{
    // Globber object to parse file names
    glob_t glob_buffer;
    // Return value for glob_buffer
    int rg = 0;
    // Flags for glob_buffer
    int flags = 0;
    // Return value for print_aliases
    int rp = 0;
    // Return value for optional arguments parsed
    int ra = 0;
    // Variable used in for loop for parsing arguments
    int i = 0;
    // Current index of glob_buffer (in pathv)
    int cur_glob_index = 0;
    // Flag to print functions or not
    bool print_functions = false;

    // Initialize flags
    flags |= GLOB_TILDE;

    // If no arguments are given or if no files are given
    // but the -f flag is present
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "-f") == 0)) {
        // Check if the -f flag is present
        print_functions = argc == 2 && strcmp(argv[1], "-f") == 0;
        // Try to glob ~/.bashrc
        rg = glob(BASHRC, flags, NULL, &glob_buffer);
        // If successful, print the aliases in the file
        if (rg == 0) {
            rp = print_aliases(glob_buffer.gl_pathv[cur_glob_index], print_functions);
            check(rp == 0, "Parsing of file `%s` failed.", glob_buffer.gl_pathv[cur_glob_index]);
            // Glob was successful, add the GLOB_APPEND to the flags so as not to
            // overwrite previous globbed filenames
            flags |= GLOB_APPEND;
            // Glob was successful, we need to increment the index
            cur_glob_index++;
        }

        // Try to glob ~/.zshrc
        rg = glob(ZSHRC, flags, NULL, &glob_buffer);
        // If successful, print the aliases in the file
        if (rg == 0) {
            rp = print_aliases(glob_buffer.gl_pathv[cur_glob_index], print_functions);
            check(rp == 0, "Parsing of file `%s` failed.", glob_buffer.gl_pathv[cur_glob_index]);
            // Glob was successful, we need to increment the index
            cur_glob_index++;
        }
    // If there are arguments to parse
    } else {
        // Parse optional arguments first
        while ((ra = getopt(argc, argv, ":vhf")) != -1) {
            switch (ra) {
                case 'v':
                    // Print version number
                    printf("\e[33m[aliases] version: %s\e[0m\n", VERSION);
                    return 0;

                case 'h':
                    // Print help screen
                    print_help_screen();
                    return 0;

                case 'f':
                    // Capture functions as well
                    print_functions = true;
                    break;

                case '?':
                default:
                    check(0, "Unknown argument `%s`. Use `aliases -h` to get a complete list of valid arguments.", argv[optind - 1]);
            }
        }

        // Parse file arguments
        for (i = optind; i < argc; i++) {
            // Skip over optional arguments that come after input files
            if (argv[i][0] != '-') {
                rp = print_aliases(argv[i], print_functions);
                check(rp == 0, "Parsing of file `%s` failed.", argv[i]);
            }
        }
    }

    // Free glob object
    if (cur_glob_index > 0) globfree(&glob_buffer);

    return 0;

error:
    // Free glob object
    if (cur_glob_index > 0) globfree(&glob_buffer);
    return 1;
}
