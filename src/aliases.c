/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#include "aliases.h"

#define SOURCE_VERSION "1.2.3" /**< The current version of `aliases` (to check along HEADER_VERSION). */

/**
 * @brief      Updates the program (by downloading and installing a newer version, if one is avalaible).
 * 
 * This function uses the `LATEST_VERSION_CMD` and `UPDATE_CMD` macros to check for the latest version, and if one is
 * available (and is newer than the current one) it downloads it and installs it, updating `aliases`.
 *
 * @return     1 if successful, 0 otherwise.
 */
int update (void)
{
    // File pointer to read output of popen() (to get latest version)
    FILE * fp;
    // Character array to store the latest version number
    char latest_version[VERSION_WIDTH + 1];
    latest_version[0] = '\0';
    // Return value of the system() call
    int ret = 0;
    // Return pointer for the fgets() call
    char * rp = NULL;

    log_info("Checking for updates...\n");

    // Try to get the latest version via curl
    fp = popen(LATEST_VERSION_CMD, "r");
    // Check if popen() call was successful
    check(fp, "Couldn't get the latest version. Check your internet connection.");
    // Get the latest version number
    rp = fgets(latest_version, VERSION_WIDTH + 1, fp);
    check(rp, "Couldn't get the latest version (version string copy failed).");
    latest_version[VERSION_WIDTH] = '\0';
    // Check that the version number is valid
    check(strlen(latest_version), "Couldn't get the latest version (version length is invalid).");

    if (strcmp(latest_version, SOURCE_VERSION) == 0) {
        // Aliases is up to date
        log_info("aliases is up to date (v%s).\n", SOURCE_VERSION);
    } else if (strcmp(latest_version, SOURCE_VERSION) < 0) {
        // Local version is ahead of latest stable release
        log_info("Your version of aliases (v%s) is ahead of the latest stable release (v%s).\n",
               SOURCE_VERSION,
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

    return 1;

error:
    if (fp) pclose(fp);
    return 0;
}


/**
 * @brief      Determines whether the specified line is a bash alias.
 *
 * @param[in]  line  The line to check.
 *
 * @return     1 if the specified line is a bash alias, 0 otherwise.
 */
int is_alias (const char * line)
{
    regex_t regex;
    int reti = 0;

    // Check line pointer
    check(line, "Invalid line (line pointer is NULL).");
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


/**
 * @brief      Determines whether the specified line is a bash function.
 *
 * @param[in]  line  The line to check.
 *
 * @return     1 if the specified line is a bash function, 0 otherwise.
 */
int is_function (const char * line)
{
    regex_t regex;
    int reti = 0;

    // Check line pointer
    check(line, "Invalid line (line pointer is NULL).");
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


/**
 * @brief      Prints an alias line.
 *
 * @param[in]  line           The line to print.
 * @param[in]  lineno         The line number.
 * @param[in]  print_linenos  Flag: if 1, print line numbers, else don't.
 *
 * @return                    1 if successful, 0 otherwise.
 */
int print_alias_line (const char * line, int lineno, int print_linenos)
{
    unsigned int i = 0;

    // Check line pointer
    check(line, "Invalid line (line pointer is NULL).");

    // Skip over the initial "alias" part
    i += strlen(ALIAS_KEYWORD) + 1;

    // Print line numbers (if the user asked for them)
    if (print_linenos)
        log_background(" %5d | ", lineno);

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

    return 1;

error:
    return 0;
}


/**
 * @brief      Prints a function line.
 *
 * @param[in]  line           The line to print.
 * @param[in]  lineno         The line number.
 * @param[in]  print_linenos  Flag: if 1, print line numbers, else don't.
 *
 * @return                    1 if successful, 0 otherwise.
 */
int print_function_line (const char * line, int lineno, int print_linenos)
{
    unsigned int i = 0;

    // Check line pointer
    check(line, "Invalid line (line pointer is NULL).");

    // Print line numbers (if the user asked for them)
    if (print_linenos)
        log_background(" %5d | ", lineno);

    // Print the name of the function
    while (line[i] != ' ' && line[i] != '\t' && line[i] != '(') {
        printf("%c", line[i]);
        i++;
    }

    // Print an arrow, then indicate that it's a function
    printf(" -> <function () {}>\n");

    return 1;

error:
    return 0;
}


/**
 * @brief      Prints aliases from a given file.
 *
 * @param[in]  filename         The file.
 * @param[in]  print_functions  Flag: if 1, print functions also, else don't.
 * @param[in]  print_linenos    Flag: if 1, print line numbers, else don't.
 *
 * @return     1 if successful, 0 otherwise.
 */
int print_aliases (const char * filename, int print_functions, int print_linenos)
{
    FILE * fp = NULL;
    char * line = NULL;
    size_t len = 0;
    int lineno = 1;
    int ret = 0;
    unsigned int i = 0;

    // Check filename
    check(filename, "Invalid filename (filename pointer is NULL).");

    // Open file to parse aliases
    fp = fopen(filename, "r");
    // Check if file exists
    check(fp != NULL, "File `%s` doesn't exist.", filename);

    // Print header message
    log_info("Aliases in file `%s`:\n", filename);
    // Print '-' characters to fit the length of the print message above
    for (i = 0; i < strlen(filename) + 20; i++) log_info("-");
    log_info("\n");

    // Read through file
    while (!feof(fp)) {
        // Get a line
        ret = getline(&line, &len, fp);
        // getline() should only return -1 if we've hit EOF
        check(ret != -1 || (ret == -1 && feof(fp)), "Failed to parse line %d.", lineno);

        // If it's an alias, print it
        if (is_alias(line)) {
            ret = print_alias_line(line, lineno, print_linenos);
            check(ret == 1, "Failed to print line %d.", lineno);
        }

        // If it's a function definition (and the user has asked for it), print it
        if (print_functions && is_function(line)) {
            ret = print_function_line(line, lineno, print_linenos);
            check(ret == 1, "Failed to print line `%d`.", lineno);
        }

        lineno++;
    }

    free(line);

    // Close the file
    fclose(fp);

    return 0;

error:
    return 1;
}


/**
 * @brief      Prints a help screen.
 */
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

    check(strcmp(HEADER_VERSION, SOURCE_VERSION) == 0,
          "Header and source versions do not match. Please reinstall `aliases`.");

    // Initialize flags
    flags |= GLOB_TILDE;

    // Parse optional arguments first
    while ((ra = getopt(argc, argv, ":vhflu")) != -1) {
        switch (ra) {
            case 'v':
                // Print version number
                log_info("[aliases] version: %s\n", SOURCE_VERSION);
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
                check(ru == 1, "Failed to update.");
                break;

            case '?':
            default:
                check(0, "Unknown argument. Use `aliases -h` to get a complete list of valid arguments.");
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
