/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"


int update (void)
{
    // File pointer to read output of popen() (to get latest version)
    FILE *fp;
    // Character array to store the latest version number
    char latest_version[VERSION_WIDTH + 1];
    latest_version[0] = '\0';
    // Return value of the system() call
    int ret = 0;

    log_info("Checking for updates...\n");

    // Try to get the latest version via curl
    fp = popen(LATEST_VERSION_CMD, "r");
    // Check if popen() call was successful
    check(fp, "Couldn't get latest version. Check your internet connection.");
    // Get the latest version number
    fgets(latest_version, VERSION_WIDTH + 1, fp);
    latest_version[VERSION_WIDTH] = '\0';
    // Check that the version number is valid
    check(strlen(latest_version), "Couldn't get latest version. Check your internet connection.");

    if (strcmp(latest_version, VERSION) == 0) {
        // Aliases is up to date
        log_info("aliases is up to date (v%s).\n", VERSION);
    } else if (strcmp(latest_version, VERSION) < 0) {
        // Local version is ahead of latest stable release
        log_info("Your version of aliases (v%s) is ahead of the latest stable release (v%s).\n",
               VERSION,
               latest_version);
    } else {
        // Newer stable version found and should be installed
        log_info("Newer stable version of aliases found: v%s\n", latest_version);
        log_info("Downloading and installing version v%s of aliases...\n", latest_version);

        // Launch the update command
        ret = system(UPDATE_CMD);
        // Check that the update command was successful
        check(ret == 0, "Error downloading version v%s.", latest_version);

        log_info("Done! Version v%s of aliases is now installed.\n", latest_version);
    }

    // Check that pclose() is successful
    check(pclose(fp) != -1, "Couldn't complete update.");

    return 0;

error:
    if (fp) pclose(fp);
    return 1;
}


// Check if a given line is an alias
int is_alias (const char * line)
{
    regex_t regex;
    int reti = 0;

    check(line, "Line pointer is NULL.");
    // Compile alias regex
    reti = regcomp(&regex, ALIAS_REGEX, REG_EXTENDED);
    check(reti == 0, "Couldn't compile alias regex.");
    // Look for regex in line
    reti = regexec(&regex, line, 0, NULL, 0);
    // Free the regex object
    regfree(&regex);
    // If the regex matches, return 1
    if (reti == 0) return 1;

error: // Fallthrough
    regfree(&regex);
    return 0;
}


int is_function (const char * line)
{
    regex_t regex;
    int reti = 0;

    check(line, "Line pointer is NULL.");
    // Compile function regex
    reti = regcomp(&regex, FUNCTION_REGEX, REG_EXTENDED);
    check(reti == 0, "Couldn't compile function regex.");
    // Look for regex in line
    reti = regexec(&regex, line, 0, NULL, 0);
    regfree(&regex);
    // If the regex matches, return 1
    if (reti == 0) return 1;

error: // Fallthrough
    regfree(&regex);
    return 0;
}


// Print a single alias line
void print_alias_line (const char * line, int lineno, int print_linenos)
{
    unsigned int i = 0;

    // Skip over the initial "alias" part
    i += strlen(ALIAS_KEYWORD) + 1;

    // Print line numbers (if the user asked for them)
    if (print_linenos)
        printf("\e[38;5;238m %5d | \e[0m", lineno);

    // Print the part that comes before the `=`
    while (line[i] != '=') {
        printf("%c", line[i]);
        i++;
    }

    // Skip over the `=`
    i++;

    // Print an arrow, then the rest of the alias
    printf(" -> ");
    // strlen(line) - 1 to avoid printing the newline at the end
    while (i < strlen(line) - 1) {
        printf("%c", line[i]);
        i++;
    }

    printf("\n");
}


// Print a single 'alias' function line
void print_function_line (const char * line, int lineno, int print_linenos)
{
    unsigned int i = 0;

    // Print line numbers (if the user asked for them)
    if (print_linenos)
        printf("\e[38;5;238m %5d | \e[0m", lineno);

    // Print the name of the function
    while (line[i] != ' ' && line[i] != '\t' && line[i] != '(') {
        printf("%c", line[i]);
        i++;
    }

    // Print an arrow, then indicate that it's a function
    printf(" -> <function () {}>\n");
}


// Print aliases in a given filename
int print_aliases (const char * filename, int print_functions, int print_linenos)
{
    FILE * fp = NULL;
    char * line = NULL;
    size_t len = 0;
    int lineno = 1;

    // Open file to parse aliases
    fp = fopen(filename, "r");
    // Check if file exists
    check(fp != NULL, "File `%s` doesn't exist.", filename);

    // Print header message
    log_info("Aliases in file `%s`:\n", filename);
    log_info("------------------------------------------\n");

    // Read through file
    while (!feof(fp)) {
        // Get a line
        getline(&line, &len, fp);

        // If it's an alias, print it
        if (is_alias(line))
            print_alias_line(line, lineno, print_linenos);

        // If it's a function definition (and the user has asked for it), print it
        if (print_functions && is_function(line))
            print_function_line(line, lineno, print_linenos);

        lineno++;
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
    log_info("Usage: aliases [options] [files]\n");
    log_info("\t-f : include function definitions\n");
    log_info("\t-l : show line numbers\n");
    log_info("\t-u : check for updates and install newer version (if available)\n");
    log_info("\t-v : print version of aliases\n");
    log_info("\t-h : print this help screen\n");
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
    // Variable used in for loop for parsing base files (~/.bashrc, ~/.bash_aliases etc)
    int i = 0;
    // Current index of glob_buffer (in pathv)
    int cur_glob_index = 0;
    // Flag to print functions or not
    int print_functions = 0;
    // Flag to print line numbers or not
    int print_linenos = 0;
    // Return value for update command
    int ru = 0;

    // Initialize flags
    flags |= GLOB_TILDE;

    // Parse optional arguments first
    while ((ra = getopt(argc, argv, ":vhflu")) != -1) {
        switch (ra) {
            case 'v':
                // Print version number
                log_info("[aliases] version: %s\n", VERSION);
                return 0;

            case 'h':
                // Print help screen
                print_help_screen();
                return 0;

            case 'f':
                // Capture functions as well
                print_functions = 1;
                break;

            case 'l':
                // Show line numbers
                print_linenos = 1;
                break;

            case 'u':
                // Update
                ru = update();
                return ru;

            case '?':
            default:
                check(0,
                      "Unknown argument `%s`. Use `aliases -h` to get a complete list of valid arguments.",
                      argv[optind - 1]);
        }
    }

    // If no file arguments are given, parse the base files (~/.bashrc, ~/.bash_aliases etc)
    if (optind > argc - 1) {
        i = 0;
        while (base_files[i]) {
            // Try to parse one of the base files
            rg = glob(base_files[i], flags, NULL, &glob_buffer);
            // If successful, print the aliases in the file
            if (rg == 0) {
                rp = print_aliases(glob_buffer.gl_pathv[cur_glob_index], print_functions, print_linenos);
                check(rp == 0, "Parsing of file `%s` failed.", glob_buffer.gl_pathv[cur_glob_index]);
                // Glob was successful, add the GLOB_APPEND to the flags so as not to overwrite previous globbed
                // filenames
                flags |= GLOB_APPEND;
                // Glob was successful, we need to increment the index
                cur_glob_index++;
            }

            i++;
        }

    // If file arguments were passed
    } else {
        // Parse file arguments
        for (i = optind; i < argc; i++) {
            // Skip over optional arguments that come after input files
            if (argv[i][0] != '-') {
                rp = print_aliases(argv[i], print_functions, print_linenos);
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
