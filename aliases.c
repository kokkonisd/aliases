/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"


// Check if a given line is an alias
bool is_alias (char * line)
{
    char * temp = "alias";
    int i = 0;

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

    // Close the file
    fclose(fp);

    return 0;

error:
    if (fp) fclose(fp);
    return 1;
}


int main (int argc, char *argv[])
{
    // Globber object to parse file names
    glob_t globber;
    // Return value for globber
    int rg = 0;
    // Return value for print_aliases
    int rp = 0;
    // Flags for globber
    int flags = 0;

    // Initialize flags
    flags |= GLOB_TILDE;

    // Try to glob ~/.bashrc
    rg = glob(BASHRC, flags, NULL, &globber);
    // If successful, print the aliases in the file
    if (rg == 0) {
        rp = print_aliases(globber.gl_pathv[0]);
        check(rp == 0, "Parsing of file `%s` failed.", globber.gl_pathv[0]);
    }

    // Try to glob ~/.zshrc
    rg = glob(ZSHRC, flags, NULL, &globber);
    // If successful, print the aliases in the file
    if (rg == 0) {
        rp = print_aliases(globber.gl_pathv[0]);
        check(rp == 0, "Parsing of file `%s` failed.", globber.gl_pathv[0]);
    }

    return 0;

error:
    return 1;
}
