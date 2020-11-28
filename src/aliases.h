/**
 * Written by Dimitris Kokkonis
 * https://github.com/kokkonisd
 **/

#ifndef __aliases_h__
#define __aliases_h__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <unistd.h>
#include <regex.h>


#define HEADER_VERSION "1.2.5" /**< The current version of `aliases` (to check along SOURCE_VERSION). */
#define VERSION_WIDTH  5       /**< The version width (3 digits + 2 dots). */

/**
 * @brief      List of base files to check in case no arguments are provided.
 */
char * base_files[] = {
    "~/.bashrc",
    "~/.bash_aliases",
    "~/.zshrc",
    "~/.zsh_aliases",
    NULL
};

/**
 * Define the grep command depending on the MACOS flag, passed by the Makefile.
 */
#ifdef MACOS
#    define GREP "ggrep"
#else
#    define GREP "grep"
#endif

/**
 * @brief       Gets the latest stable version of `aliases` from GitHub.
 * 
 * This command uses curl and grep to grab the latest version of `aliases` from its GitHub repo.
 */
#define LATEST_VERSION_CMD "curl -s https://api.github.com/repos/kokkonisd/aliases/releases/latest | "\
                           GREP " -oP '(?<=\"tag_name\": \"v)[0-9.]+(?=\")'"

/**
 * @brief      Downloads & installs the latest stable version of `aliases` from GitHub.
 * 
 * This command uses curl, grep, tar and make to download the latest version of `aliases` from its GitHub repo, unpack
 * it in the /tmp/ folder, install it and clean up.
 */
#define UPDATE_CMD         "TMPDWN=$(curl -s https://api.github.com/repos/kokkonisd/aliases/releases/latest | "\
                           GREP " -oP '(?<=\"tarball_url\": \").+(?=\")') && "\
                           "curl -L -s $TMPDWN -o /tmp/aliases_latest.tar.gz && "\
                           "tar -xvzf /tmp/aliases_latest.tar.gz -C /tmp/ && "\
                           "cd /tmp/kokkonisd-aliases-* && "\
                           "make clean && "\
                           "sudo make uninstall && "\
                           "sudo make install && "\
                           "cd .. && "\
                           "rm -rf kokkonisd-aliases-* aliases_latest*"


#define ALIAS_KEYWORD "alias" /**< The alias keyword (used to calculate the word's length as well as the regex). */

#define ALIAS_REGEX    "^" ALIAS_KEYWORD "[ \\t]+.+=.+" /**< Regex to detect bash alias definitions. */
#define FUNCTION_REGEX "^.+[ \\t]*\\(.*\\)[ \\t]*\\{"   /**< Regex to detect bash function definitions. */

/**
 * @brief      Logs an info message.
 * 
 * This macro prints a colored info message to `stdout`.
 *
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 */
#define log_info(M, ...) fprintf(stdout, "\e[33m" M "\e[0m", ##__VA_ARGS__);

/**
 * @brief      Logs a background message.
 * This macro prints a colored "background" message to `stdout`.
 *
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 */
#define log_background(M, ...) fprintf(stdout, "\e[38;5;238m" M "\e[0m", ##__VA_ARGS__);

/**
 * @brief      Logs an error message.
 * 
 * This macro prints a colored error message to `stderr`.
 *
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 */
#define log_err(M, ...) fprintf(stderr, "\e[31m" M "\e[0m\n", ##__VA_ARGS__)

/**
 * @brief      Checks a condition and produces an error if false.
 * 
 * This macro checks a condition. If the condition is false, an appropriate error message is printed, and then the
 * function jumps down to its `error` section to handle the error.
 *
 * @param      A     A condition to check.
 * @param      M     A message string, containing formatting characters.
 * @param      ...   Potential arguments to the string.
 *
 * @return     { description_of_the_return_value }
 */
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); goto error; }


#endif
