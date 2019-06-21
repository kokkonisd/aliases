/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"


// Check if a given line is an alias
bool is_alias (const char * line)
{
    char * temp = "alias";
    int i = 0;

    if (!line) return false;

    // Compare "alias" and given line
    for (i = 0; i < strlen(temp); i++)
        if (temp[i] != line[i]) return false;

    return true;
}


// Print a single alias line
void print_alias_line (char * line)
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


// Print aliases in a given filename
int print_aliases (char * filename)
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

    // Initialize flags
    flags |= GLOB_TILDE;

    // If no arguments are given
    if (argc == 1) {
        // Try to glob ~/.bashrc
        rg = glob(BASHRC, flags, NULL, &glob_buffer);
        // If successful, print the aliases in the file
        if (rg == 0) {
            rp = print_aliases(glob_buffer.gl_pathv[cur_glob_index]);
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
            rp = print_aliases(glob_buffer.gl_pathv[cur_glob_index]);
            check(rp == 0, "Parsing of file `%s` failed.", glob_buffer.gl_pathv[cur_glob_index]);
        }
    // If there are arguments to parse
    } else {
        // Parse optional arguments first
        while ((ra = getopt(argc, argv, ":vh")) != -1) {
            switch (ra) {
                case 'v':
                    // Print version number
                    printf("\e[33m[aliases] version: %s\e[0m\n", VERSION);
                    return 0;

                case 'h':
                    // Print help screen
                    print_help_screen();
                    return 0;

                case '?':
                default:
                    check(0, "Unknown argument `%s`. Use `aliases -h` to get a complete list of valid arguments.", argv[optind - 1]);
            }
        }

        // Parse file arguments
        for (i = optind; i < argc; i++) {
            // Skip over optional arguments that come after input files
            if (argv[i][0] != '-') {
                rp = print_aliases(argv[i]);
                check(rp == 0, "Parsing of file `%s` failed.", argv[i]);
            }
        }
    }

    // Free glob object
    if (glob_buffer.gl_pathc) globfree(&glob_buffer);

    return 0;

error:
    // Free glob object
    if (glob_buffer.gl_pathc) globfree(&glob_buffer);
    return 1;
}
